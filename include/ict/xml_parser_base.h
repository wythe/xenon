#pragma once
//-- Copyright 2015 Intrig
//-- see https://github.com/intrig/xenon for license
#include <ict/ict.h>

#include <string>
#include <iostream>
#include <sstream>

// macro to throw error with "expected X before Y token" description 
#define THROW_EXPECTED(X, Y) \
do { \
    std::ostringstream os; \
    os << "expected '" << (X) << "' before '" << (Y) << "' token"; \
    ict::exception e(os.str().c_str()); \
    e.xml_line = line(); \
    e.xml_column = column(); \
    throw e; \
} while (0);

// macro to throw error with "unexpected X token" description 
#define THROW_UNEXPECTED(X) \
do { \
    std::ostringstream os; \
    os << "unexpected '" << (X) << "' token"; \
    ict::exception e(os.str().c_str()); \
    e.xml_line = line(); \
    e.xml_column = column(); \
    throw e; \
} while (0);

namespace ict {

/* http://www.w3.org/TR/2006/REC-xml-20060816/ */
class xml_parser_base
{
    public:

    xml_parser_base();

    // TODO: write copy constructor and copy assignment
    
    xml_parser_base(const xml_parser_base & b);

    xml_parser_base & operator=(const xml_parser_base & b);

    virtual ~xml_parser_base();

    /* Prepare a parser object to be re-used.  This is particularly
       valuable when memory allocation overhead is disproportionatly high,
       such as when a large number of small documents need to be parsed.
       The state of the parser is simply reset to the initial state.
    */
    void reset();

    /* Parses some input. Throws ParseException.h if a fatal error is
       detected.  The last call to parse() must have final true; len may be
       zero for this call.
    */
    void parse(const char * s, int len, bool final);

    /* These functions return information about the current parse
       location.  This location refers to the last character parsed.  For
       example, in a startElement() or endElement() handler, the current
       byte location will be referencing the '>' character.
    */
    int line() const { return _line; }
    int column() const { return _column; }
    int byte() const { return _byte; }

    /* Return true if parse() is called after it was called with the final
     * flag set */
    bool finished() const { return _finished; }

/* Stops parsing, causing parse() to return.
   Must be called from within a handler.  Some handlers may still follow
   because they would otherwise get lost. For example, for empty elements,
   endElement() will still be invoked if stop() is called from
   startElement(). 

   If a stopped parser is stopped again, an InvalidState Exception.h will be
   thrown.

   When suspended, parsing can be resumed by calling resume(). resume() can
   possibly throw an InvalidStated exception.
*/
    void stop();
    void resume();

    protected:
    // handlers
    virtual void startElement(const char * s, const char ** atts) = 0;
    virtual void endElement(const char * s) = 0;
    virtual void characterData(const char * s) = 0;
    virtual void comment(const char * s) = 0;
    virtual void proc_inst(const char *) {};

    enum { MaxAtts = 50 };
    enum { MaxCDataSize = 1024 * 10 };
    enum { MaxCommentSize = 1024 * 10 };
    enum { MaxTagDepth = 100 };
    enum { MaxTagSize = 1024 };
    enum { MaxAttSize = 1024 * 10 };
        
    // update the state_names array in the source file if you change this
    enum State 
    {
        First, // looking for S or first '<'
        StateStart, // parsing cdata, looking for '<' or '>'
        Start1, // found '<', looking for [!?/] or tag start char

        CommentStart1, // "<!"
        CommentStart2, // "<!-"

        CommentStart3, // "<!--"
        
        CommentBody, // (parsing comment string)
        CommentEnd1, // "-"
        CommentEnd2, // "--"

        PI, // looking for '?'

        PIEnd1, // looking for '>'

        // STag ::= '<' Name (S Attribute)* S? '>'
        
        StartTag1, // (parsing tag Name)

        FirstAtt1, // looking for first Attribute start char or '>' or S 
        FirstAtt2, // found S, looking for first Attribute start char or S

        AttName1, // looking for subsequent Attribute start char or '>' or S
        AttName2, // found S, looking for subsequent Attribute start char
                  // or S or '>'
        AttName2a, // found S, looking for subsequent Attribute start char or S

        AttName3, // reading attribute name, looking for '=' or S
        AttName4, // looking for '=' or S

        AttValue1, // looking for starting '"'
        AttValue2, // looking for att value start char
        AttValue3, // reading att value, looking for ending '"'

        Empty, // / (just read '/' and looking for '>')

        // ETag ::=  '</' Name S? '>'
        EndTag1, // looking for end tag Name start char
        EndTag2, // parsing end tag Name
        EndTag3, // looking for '>'

        Epilog, // finished reading end root element tag
        Epilog1
    };

    private:

    inline bool isNameStartChar(int c)
    {
        return (isalpha(c) || (c=='_') || (c == ':'));
    }

    inline bool isNameChar(int c)
    {
        return (isalnum(c)||(c=='.')||(c=='-')||(c=='_')||(c==':'));
    }

    inline bool isAttValueChar(int c)
    {
        return (!((c=='^') || (c=='"')));
    }

    inline void nextAtt() { _ac++; _ai++; _a = *_ai; }
    inline void resetAtts() { _ac=0; _ai = _atts; _a = *_ai; *_a = '\0'; }
    inline void lastAtt() { _ac++; _ai++; *(*_ai) = '\0'; }

    inline void resetTags() { _tc=0; _ti = _tags; _t = *_ti; *_t = '\0'; }
    inline void pushTag() { _tc++; _ti++; _t = *_ti; }
    inline void popTag() { _tc--; _ti-- ; _t = *_ti; }

    // handle &lt; &gt; &amp; &quot; &apos;
    enum CharState {
        Idle,
        Entity, // found '&', looking for ';' reading entity reference

        LT1, // "l"
        GT1, // "g"
        AMP1, // "a"

        LT2, // "lt"
        GT2, // "gt"
        AMP2, // "am"
        AMP3, // "amp"

        QUOT1, // q
        QUOT2, // qu
        QUOT3, // quo
        QUOT4, // quot
        
        APOS2, // ap
        APOS3, // apo
        APOS4, // apos
    };

    /* This will work with a char * or an iterator */
    template <typename T>
    T addChar(CharState *s, T d)
    {
        switch (*s)
        {
            case Idle:
                switch (*_ch) {
                    case '&' : *s = Entity; break;
                    default: 
                    {
                        *d++=*_ch;
                    }
                }
                break;
            case Entity:
                switch (*_ch) {
                    case 'l' : *s = LT1; break;
                    case 'g' : *s = GT1; break;
                    case 'a' : *s = AMP1; break;
                    case 'q' : *s = QUOT1; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;

            // lt;
            case LT1:
                switch (*_ch) {
                    case 't' : *s = LT2; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            case LT2:
                switch (*_ch) {
                    case ';' : *s = Idle; *d++ = '<'; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;

            // gt;
            case GT1:
                switch (*_ch) {
                    case 't' : *s = GT2; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            case GT2:
                switch (*_ch) {
                    case ';' : *s = Idle; *d++ = '>'; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;

            // amp;
            case AMP1:
                switch (*_ch) {
                    case 'm' : *s = AMP2; break;
                    case 'p' : *s = APOS2; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            case AMP2:
                switch (*_ch) {
                    case 'p' : *s = AMP3; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            case AMP3:
                switch (*_ch) {
                    case ';' : *s = Idle; *d++ = '&'; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;

            // quot
            case QUOT1:
                switch (*_ch) {
                    case 'u' : *s = QUOT2; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            case QUOT2:
                switch (*_ch) {
                    case 'o' : *s = QUOT3; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            case QUOT3:
                switch (*_ch) {
                    case 't' : *s = QUOT4; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            case QUOT4:
                switch (*_ch) {
                    case ';' : *s = Idle; *d++ = '"'; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            
            // apos
            case APOS2:
                switch (*_ch) {
                    case 'o' : *s = APOS3; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            case APOS3:
                switch (*_ch) {
                    case 's' : *s = APOS4; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;
            case APOS4:
                switch (*_ch) {
                    case ';' : *s = Idle; *d++ = '\''; break;
                    default : THROW_UNEXPECTED(*_ch);
                }
                break;

            default:
                THROW_UNEXPECTED(*_ch);
        }
        return d;
    }

    inline void set(State s) { _state = s; }
    inline State state() { return _state; }

    const char *_ch;  // current char

    std::vector<char> _cdata;
    std::vector<char>::iterator _d;

    std::vector<char> _comment;
    std::vector<char>::iterator _c;

    std::vector<char> _proc_inst;
    std::vector<char>::iterator _pi;

    char ** _tags; // tag stack
    char ** _ti; // tag iterator
    char * _t;
    int _tc; // tag depth count (for debugging)

    char * _e; // end tag char pointer

    char ** _atts; // attribute list and values
    char ** _ai; // atribute iterator
    char *_a; 
    int _ac; // att count (for debugging)

    //int _error;
    int _line;
    int _column;
    int _byte;
    bool _done;

    State _state;
    CharState _cd_state;
    CharState _att_state;

    bool _finished;
    bool _suspended;
};

// This used to be in a cpp file
// order matters here, index is state()
static const char * state_names[] = {
        "First",
        "StateStart",
        "Start1",
        "CommentStart1",
        "CommentStart2",
        "CommentStart3",
        "CommentBody",
        "CommentEnd1",
        "CommentEnd2",
        "PI",
        "PIEnd1",
        "StartTag1",
        "FirstAtt1",
        "FirstAtt2",
        "AttName1",
        "AttName2",
        "AttName2a",
        "AttName3",
        "AttName4",
        "AttValue1",
        "AttValue2",
        "AttValue3",
        "Empty",
        "EndTag1",
        "EndTag2",
        "EndTag3",
        "Epilog",
        "Epilog1"
    };

inline xml_parser_base::xml_parser_base()
{
    int i;
    // init once stuff
    _atts = new char * [MaxAtts];
    if (!_atts) { IT_PANIC("out of memory"); }
    for (i=0; i<MaxAtts; ++i) 
    {
        _atts[i] = new char [MaxAttSize];
        if (!_atts[i]) IT_PANIC("out of memory:" << i);
    }
    
    _tags = new char * [MaxTagDepth];
    if (!_tags) IT_PANIC("out of memory");
    for (i=0; i<MaxTagDepth; ++i) 
    {
        _tags[i] = new char [MaxTagSize];
        if (!_tags[i]) IT_PANIC("out of memory:" << i);
    }
    
    reset();
}

// this doen't make sense since its pretty much impossible to copy the state of the parser right now
// but at least we won't get any bad deallocs
inline xml_parser_base::xml_parser_base(const xml_parser_base &) : xml_parser_base() {
}

inline xml_parser_base & xml_parser_base::operator=(const xml_parser_base &) {
    reset();
    return *this;
}

inline xml_parser_base::~xml_parser_base()
{
    int i;
    for (i=0; i<MaxAtts; ++i) 
    {
        delete [] _atts[i];
    }
    delete [] _atts;
    
    for (i=0; i<MaxTagDepth; ++i) 
    {
        delete [] _tags[i];
    }
    delete [] _tags;
    
}

inline void xml_parser_base::stop()
{
    if (_finished) IT_PANIC("cannot stop a finished parser");
    if (_suspended) IT_PANIC("cannot suspend a suspended parser");
    _suspended = true;
}

inline void xml_parser_base::resume()
{
    if (_finished) IT_PANIC("cannot resume a finished parser");
    if (!_suspended) IT_PANIC("cannot resume a running parser");
    _suspended = false;
}

inline void xml_parser_base::reset()
{
    _done = false;
    _finished = false;

    _line=1;
    _column=0;
    _byte=0;

    _cdata.assign(MaxCDataSize, '\0');
    _d = _cdata.begin();

    _comment.assign(MaxCommentSize, '\0');
    _c = _comment.begin();
    
    _proc_inst.assign(MaxCommentSize, '\0');
    _pi = _proc_inst.begin();

    resetTags();
    resetAtts();

    _state = First;
    _cd_state = Idle;
    _att_state = Idle;

    _suspended = false;
}
// See http://www.w3.org/TR/REC-xml/REC-xml-20040204.xml for implementation
// details.
inline void xml_parser_base::parse(const char * s, int len, bool final)
{
    if (_finished) IT_PANIC("finished parsing");
    if (_suspended) IT_PANIC("parsing suspended");
    if (!len) return;
    if (final) _finished = true;

    for (_ch = s; _ch != (s + len); ++_ch)
    {
        ++_byte;
        if (*_ch == '\n')
        {
            ++_line;
            _column = 0;
        } else {
            ++_column;
        }

        switch (state())
        {
            case First: // looking for first tag
                switch (*_ch) {
                    case ' ': case '\n': case '\r': case '\t': break;
                    case '<' : // starting a tag
                        set(Start1); break;
                    default :
                        THROW_EXPECTED("whitespace or '<'", *_ch);
                }
                break;
            case StateStart: // expect a start tag
                switch (*_ch) {
                    case '<' : // starting a tag
                        // if there is cdata, then call handler
                        if (_d != _cdata.begin())
                        {
                            *_d = '\0';
                            characterData(&_cdata[0]);
                            _d = _cdata.begin();
                        }
                        set(Start1); break;
                    case '>' : THROW_UNEXPECTED(*_ch);
                    default :  // just cdata
                        _d = addChar(&_cd_state, _d);
                }
                break;
            case Start1 :
                switch (*_ch) {
                    case '!' : set(CommentStart1); break;
                    case '?' : set(PI); break;
                    case '/' : 
                        set(EndTag1); break;
                    default: // must be a start of an element name
                        if (isNameStartChar(*_ch))
                        {    
                            pushTag();
                            *_t++ = *_ch;
                            set(StartTag1);
                        } else THROW_EXPECTED("start char", *_ch);
                }
                break;
            case StartTag1 :
                if (isNameChar(*_ch)) *_t++ = *_ch;
                else switch (*_ch) {
                    case ' ': case '\n': case '\r': case '\t':
                        *_t = '\0';
                        _t = *_ti; // _t now holds the tag name
                        set(FirstAtt1);
                        break;
                    case '/':  // we have an empty element with no attributes
                        *_t = '\0';
                        _t = *_ti; // _t now holds the tag name
                        resetAtts();
                        set(Empty);
                        break;
                    case '>':  // done, call handler with no attributes
                        *_t = '\0';
                        _t = *_ti; // _t now holds the tag name
                        resetAtts();
                        startElement(_t, (const char **) _atts);
                        set(StateStart);
                        break;
                    default:
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case FirstAtt1 : 
                if (isNameStartChar(*_ch)) 
                {
                    resetAtts();
                    _a = addChar(&_att_state, _a);
                    set(AttName3);
                } else switch (*_ch) {
                    case ' ': case '\n': case '\r': case '\t': 
                        set(FirstAtt2);
                        break;
                    case '>':  // (same as above case)
                        resetAtts();
                        startElement(_t, (const char **) _atts);
                        set(StateStart);
                        break;
                    default: 
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case FirstAtt2 : 
                if (isNameStartChar(*_ch)) 
                {
                    resetAtts();
                    _a = addChar(&_att_state, _a);
                    set(AttName3);
                } else switch (*_ch) {
                    case ' ': case '\n': case '\r': case '\t': break;
                    default: 
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case AttName1 :
                if (isNameStartChar(*_ch)) 
                {
                    nextAtt();
                    _a = addChar(&_att_state, _a);
                    set(AttName3);
                } 
                else switch (*_ch) {
                    case '/' : set(Empty); break;
                    case ' ': case '\n': case '\r': case '\t': 
                        set(AttName2);
                        break;
                    case '>':  // done, call handler with attributes
                        lastAtt();
                        startElement(_t, (const char **) _atts);
                        resetAtts();
                        set(StateStart);
                        break;
                    default : 
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case AttName2 :
                if (isNameStartChar(*_ch)) 
                {
                    nextAtt();
                    _a = addChar(&_att_state, _a);
                    set(AttName3);
                } else switch (*_ch) {
                    case '/' : set(Empty); break;
                    case ' ': case '\n': case '\r': case '\t': 
                        set(AttName2a);
                        break;
                    case '>':  // done, call handler with attributes
                        lastAtt();
                        startElement(_t, (const char **) _atts);
                        resetAtts();
                        set(StateStart);
                        break;
                    default:
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case AttName2a :
                if (isNameStartChar(*_ch)) 
                {
                    nextAtt();
                    _a = addChar(&_att_state, _a);
                    set(AttName3);
                } else switch (*_ch) {
                    case ' ': case '\n': case '\r': case '\t':  
                        break;
                    default:
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case AttName3 :
                if (isNameChar(*_ch)) {
                    _a = addChar(&_att_state, _a);
                }
                else switch (*_ch) {
                    case ' ': case '\n': case '\r': case '\t':
                        *_a = '\0'; // _a now holds the tag name
                        set(AttName4);
                        break;
                    case '=': 
                        *_a = '\0'; // _a now holds the tag name
                        set(AttValue1);
                        break;
                    default:
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case AttName4 :
                switch (*_ch) {
                    case ' ': case '\n': case '\r': case '\t':
                        break;
                    case '=': 
                        set(AttValue1);
                        break;
                    default:
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case AttValue1 : 
                switch (*_ch) {
                    case ' ': case '\n': case '\r': case '\t': break;
                    case '"' : set(AttValue2); break;
                    default:
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case AttValue2 :
                if (isAttValueChar(*_ch)) 
                {
                    nextAtt();
                    _a = addChar(&_att_state, _a);
                    set(AttValue3);
                } else 
                    THROW_UNEXPECTED(*_ch);
                break;
            case AttValue3 : 
                if (isAttValueChar(*_ch)) 
                {
                    _a = addChar(&_att_state, _a);
                }
                else switch (*_ch) {
                    case '"':
                        *_a = '\0';
                        set(AttName1);
                        break;
                    default:
                        THROW_UNEXPECTED(*_ch);
                }
                break;
            case Empty :
                switch (*_ch) {
                    case '>' :
                        lastAtt();
                        startElement(_t, (const char **) _atts);
                        endElement(_t);
                        if (_ti==(_tags+1)) set(Epilog);
                        else {
                            popTag();
                            set(StateStart);
                        }
                        break;
                    default: THROW_EXPECTED('>', *_ch);
                }
                break;
            case EndTag1:
                if (*_ch == *_t)
                {
                    _e = (_t + 1);
                    set(EndTag2);
                } else THROW_EXPECTED(*_t, *_ch);
                break;
            case EndTag2 :
                if (*_ch != *_e) 
                {
                    switch (*_ch) {
                        case ' ': 
                        case '\n': 
                        case '\r': 
                        case '\t':
                            if (*_e == '\0') set(EndTag3);
                            else IT_PANIC("tag mismatch");
                            break;
                        case '>' : 
                            if (*_e == '\0') 
                            {
                                endElement(_t);
                                if (_ti==(_tags+1)) 
                                {
                                    set(Epilog);
                                }
                                else {
                                    popTag();
                                    set(StateStart);
                                }
                            } else THROW_EXPECTED("\\0", *_e);
                            break;
                        default:
                            THROW_EXPECTED(*_e, *_ch);
                            
                    }
                } else _e++;
                break;
            case EndTag3 :
                switch (*_ch) {
                    case '>' :
                        endElement(_t);
                        if (_ti==(_tags+1)) set(Epilog);
                        else {
                            popTag();
                            set(StateStart);
                        }
                        break;
                    default: THROW_EXPECTED('>', *_ch);
                }
                break;
            case CommentStart1 : 
                if (*_ch == '-') set(CommentStart2);
                else THROW_EXPECTED('-', *_ch);
                break;
            case CommentStart2 : 
                if (*_ch == '-') set(CommentStart3);
                else THROW_EXPECTED('-', *_ch);
                break;
            case CommentStart3 : 
                if (*_ch == '-') IT_PANIC("\"<!---\" is invalid XML");
                *_c++ = *_ch;
                set(CommentBody); 
                break;
            case CommentBody : 
                *_c++ = *_ch;
                if (*_ch == '-') set(CommentEnd1);
                break;
            case CommentEnd1 : 
                *_c++ = *_ch;
                if (*_ch == '-') set(CommentEnd2);
                else set(CommentBody);
                break;
            case CommentEnd2 : 
                *_c++ = *_ch;
                if (*_ch == '>') 
                {
                    _c -=3;
                    *_c = '\0';
                    comment(&_comment[0]);
                    _c = _comment.begin();
                    if (_done) set(Epilog);
                    else set(StateStart); // we're done with comment
                }
                else IT_PANIC("\"--\" cannot occur within a comment");
                break;
            case PI : 
                *_pi ++ = *_ch;
                if (*_ch == '?') set(PIEnd1);
                break;
            case PIEnd1:
                if (*_ch == '>') 
                {
                    if (_done) set(Epilog);
                    else set(StateStart);
                    --_pi;
                    *_pi = '\0';
                    proc_inst(&_proc_inst[0]);
                    _pi = _proc_inst.begin();
                }
                else THROW_EXPECTED('>', *_ch);
                break;
            case Epilog: // only comments or PIs allowed
                _done = true;
                switch (*_ch) {
                    case ' ': case '\n': case '\r': case '\t': break;
                    case '<' : set(Epilog1); break;
                    default : IT_PANIC("junk after doc element", "", line(), column());
                }
                break;
            case Epilog1:
                switch (*_ch) {
                    case '!' : set(CommentStart1); break;
                    case '?' : set(PI); break;
                    default : IT_PANIC("junk after doc element", "", line(), column());
                }
                break;
        }
    }

    if (_finished && (state() != Epilog)) 
        IT_PANIC("parsing incomplete");
        
    return;
}

}
