#  XDDL Element Reference
* 1 [Elements](#Elements)
    * 1.1 [bit](#bit)
    * 1.2 [case](#case)
    * 1.3 [comment](#comment)
    * 1.4 [cstr](#cstr)
    * 1.5 [default](#default)
    * 1.6 [enc](#enc)
    * 1.7 [export](#export)
    * 1.8 [field](#field)
        * 1.8.1 [bias](#bias)
        * 1.8.2 [type](#type)
    * 1.9 [fragment](#fragment)
    * 1.10 [if](#if)
    * 1.11 [item](#item)
    * 1.12 [jump](#jump)
    * 1.13 [oob](#oob)
    * 1.14 [pad](#pad)
        * 1.14.1 [mod](#mod)
    * 1.15 [peek](#peek)
    * 1.16 [prop](#prop)
    * 1.17 [range](#range)
    * 1.18 [record](#record)
        * 1.18.1 [record definition](#recorddefinition)
        * 1.18.2 [record link](#recordlink)
    * 1.19 [repeat](#repeat)
        * 1.19.1 [repeat indefinitely](#repeatindefinitely)
        * 1.19.2 [repeat based on an expression](#repeatbasedonanexpression)
        * 1.19.3 [bound repeat](#boundrepeat)
    * 1.20 [script](#script)
    * 1.21 [setprop](#setprop)
    * 1.22 [start](#start)
    * 1.23 [switch](#switch)
    * 1.24 [type](#type)
    * 1.25 [uint16](#uint16)
    * 1.26 [uint32](#uint32)
    * 1.27 [uint64](#uint64)
    * 1.28 [uint8](#uint8)
    * 1.29 [while](#while)
    * 1.30 [xddl](#xddl)
* 2 [Attribute Types](#AttributeTypes)

<h2 id="Elements">1 Elements</h2>
<h2 id="bit">1.1 bit</h2>

A [bit](#bit) is a bit.


attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004;  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)


The following example is a one bit message:

    <xddl>
      <bit name="x"/>
    </xddl>
And decoding a bit with [idm](#idm):

    # idm obp.xddl @1
    
    Name  Length  Value  Hex  Description
    x     1       1      @1
<h2 id="case">1.2 case</h2>

The [case](#case) element only appears as a child of the [switch](#switch) element.  It is similar to the *case* keyword in 
*C++*.

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | value | integer| &#10004;  | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)


See the [switch](#switch) element for example usage.
<h2 id="comment">1.3 comment</h2>

attributes: none



children: none

<h2 id="cstr">1.4 cstr</h2>

The [cstr](#cstr) element represents a null-terminated C string.

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | max | expression| | 
 | name | string| &#10004;  | 


children: none



Example:

    <xddl>
      <cstr name="greeting"/>
    </xddl>
Decoding the ASCII hex for "Hello" yields:

    # idm cstr.xddl 48656C6C6F00
    
    Name     Length  Value          Hex           Description
    greeting 48      79600447942400 #48656C6C6F00 Hello
<h2 id="default">1.5 default</h2>

The [default](#default) element only appears as a child of the [switch](#switch) element.  It is similar to the *default* keyword in 
*C/C++*.

attributes: none



children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)



See the [switch](#switch) element for example usage.
<h2 id="enc">1.6 enc</h2>

The [enc](#enc) element is used to encapsulate encoding fields. Encoding fields are by default not displayed in the [idm](#idm). 

attributes: none



children: [fragment](#fragment), [type](#type), [start](#start), [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)



Here is an example that reads a `size` field and then a field of that length.  
In this case, we consider `size` to be an encoding field, and not an important part of the message for display 
purposes.  We surround it with [enc](#enc) to indicate so.

    <xddl>
      <enc>
        <uint8 name="size"/>
      </enc>
      <field name="value" length="size"/>
    </xddl>
And decoding `080F` with the [idm](#idm) skips over the `length` field:

    # idm enc.xddl 080F
    
    Name  Length  Value  Hex  Description
    value 8       15     #0F
But running [idm](#idm) with the `--encoding` flag will display it:

    # idm --encoding enc.xddl 080F
    
    Name  Length  Value  Hex  Description
    size  8       8      #08
    value 8       15     #0F
However, records inserted within an encoding range are themselves not considered to be encoding.
<h2 id="export">1.7 export</h2>

[export](#export) provides a way to create global properties in a message.  These properties can be used and set by different 
records as a message is being parsed.  

attributes: none



children: [prop](#prop)



The following file creates a global property, *size*.  The *A* record references this size and creates a field
based on its length.  The *B* record also creates a field based on the value of *size*, but since there is a local
*size* property defined in its scope (with the value of 16), it is used instead.

    <xddl>
      <export>
        <prop name="size" value="8"/>
      </export>
      <record id="A">
        <field name="b" length="size"/>
      </record>
      <record id="B">
        <prop name="size" value="16"/>
        <field name="b" length="size"/>
      </record>
      <start>
        <record name="A" href="#A"/>
        <record name="B" href="#B"/>
      </start>
    </xddl>
Decoding three bytes yields:

    # idm export.xddl 010203
    
    Name  Length  Value  Hex   Description
    A
      b   8       1      #01
    B
      b   16      515    #0203
These properties are also visible to records that are included from different files.  There are no "file local" 
properties.

<h2 id="field">1.8 field</h2>

The [field](#field) element identifies an integer unit of information specific to the message being represented.  

It must have a `name` and `length` attribute.  The `length` is specified in bits, and may be any nonnegative integer
value.  It does not have to be byte aligned within the record it appears.

The optional `bias` attribute is added to the value by a fixed amount when displayed in the [idm](#idm).  See the `bias`
example in the description below.

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004;  | 
 | type | url| | 
 | bias | integer| | 
 | length | expression| &#10004;  | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)



This is a simple example that defines a field named "foo" and is 4 bits long.  The `name` and `length` are required
attributes, and typically they are the only ones used.  Here is an example describing a simple message consisting of one
4 bit field.

    <xddl>
      <start>
        <field name="sequence" length="4"/>
      </start>
    </xddl>
Parsing the four bit message "@1111" results in:

    # idm simple_field.xddl @1111
    
    Name     Length  Value  Hex   Description
    sequence 4       15     @1111
<h2 id="bias">1.8.1 bias</h2>


The optional `bias` attribute is used to offset the value of field by a
fixed amount.  Here's an example:

    <xddl>
      <field name="a" length="1" bias="-10"/>
      <field name="b" length="1" bias="-9"/>
      <field name="c" length="1" bias="-8"/>
      <field name="d" length="1" bias="-7"/>
      <field name="e" length="1" bias="1"/>
      <field name="f" length="1" bias="2"/>
      <field name="g" length="1" bias="3"/>
      <field name="h" length="1" bias="4"/>
    </xddl>
Each field is just 1 bit long, but we are biasing them by varying amounts.
The bias is applied after the fields are parsed.  If we parse a message of
all zeroes, here is what we get:

    # idm bias.xddl @00000000
    
    Name  Length  Value  Hex  Description
    a     1       -10    @0
    b     1       -9     @0
    c     1       -8     @0
    d     1       -7     @0
    e     1       1      @0
    f     1       2      @0
    g     1       3      @0
    h     1       4      @0
As you can see, the *Value* column is offset by the `bias`.  The *Hex*
column still reflects the original bit pattern.

<h2 id="type">1.8.2 type</h2>


The optional `type` attribute references a [type](#type) element's `id`.  See the [type](#type) element
reference for examples.

This example references a locally defined [type](#type).  See the [type](#type) documentation for more information.

    <xddl>
     <type id="HelloType">
        <item key="0" value="Goodbye World!"/>
        <item key="1" value="Hello World!"/>
     </type>
     <bit name="A" type="#HelloType"/>
     <bit name="B" type="#HelloType"/>
    </xddl>
And decoding the bits `10` yields:

    # idm hello.xddl @10
    
    Name  Length  Value  Hex  Description
    A     1       1      @1   Hello World!
    B     1       0      @0   Goodbye World!
<h2 id="fragment">1.9 fragment</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | href | url| &#10004;  | 


children: none



The following example parses the same record twice, once as a fragment, and then once as a record.

    <xddl>
      <record id="A">
        <field name="b" length="8"/>
      </record>
      <start>
        <fragment href="#A"/>
        <record name="A" href="#A"/>
      </start>
    </xddl>
The result:

    # idm fragment.xddl 0102
    
    Name  Length  Value  Hex  Description
    b     8       1      #01
    A
      b   8       2      #02
Fragments are useful sometimes when many messages contain the same handfull of fields.

<h2 id="if">1.10 if</h2>


The [if](#if) element provides a way to conditionally include other elements based
on an *expression*.


attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | expr | expression| &#10004;  | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)



The following example illustrates the conditional inclususion of a field:

    <xddl>
      <start>
        <field name="Included" length="8"/>
        <if expr="Included">
         <field name="More" length="8"/>
        </if>
      </start>
    </xddl>
Now we parse two messages with the above file.  The first one will
include the `More` field and the second one will not:

    # idm if.xddl 0105 00
    
    Name     Length  Value  Hex  Description
    Included 8       1      #01
    More     8       5      #05
    Name     Length  Value  Hex  Description
    Included 8       0      #00
The `expr` attribute may be any XDDL expression.  As long as it does not
evaluate to zero, the conditional elements will be included.

<h2 id="item">1.11 item</h2>


The [item](#item) element only appears as a child of the [type](#type) element.  It is
used to specify an item of an enumerated list.



attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | key | integer| &#10004;  | 
 | href | url| | 
 | value | string| &#10004;  | 


children: [comment](#comment)


The option *url* attribute can be specified and is used in conjuntion with the [jump](#jump) element.

See [type](#type) for example usage.
<h2 id="jump">1.12 jump</h2>

A [jump](#jump) element provides an easy way to choose a record to parse based on a value.

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | base | jump_name| &#10004;  | 


children: none


A common pattern among parsing messages is to choose one of many records to parse based on a single field's value, a
message type, for example.  This can easy enough be done with a [switch](#switch) element:

    <uint8 name="msg-id"/>
    <switch expr="msg-id">
        <case value="1">
            <record href="#A"/>
        </case>
        <case value="2">
            <record href="#B"/>
        </case>
        <case value="3">
            <record href="#C"/>
        </case>
          .
          .
          .
    </switch>

Using [jump](#jump) along with [type](#type) can greatly simplify this trivial case:

    <uint8 name="msg-id" type="#msg-id"/>
    <type id="msg-type">
      <item key="1" value="A" href="#A"/>
      <item key="2" value="B" href="#B"/>
      <item key="3" value="C" href="#C"/>
         .
         .
         .
    </type>
    <jump base="msg-id"/>

The above two listings are functionally equivalent.
<h2 id="oob">1.13 oob</h2>

[oob](#oob) is used to indicate out-of-band data.  It is functionally equivalent to [enc](#enc).

attributes: none



children: [type](#type), [start](#start), [export](#export), [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="pad">1.14 pad</h2>


The [pad](#pad) element is used to align a record to a boundary.  Typically, this
will be a byte boundary, but can be changed by using the attributes.

It's length is not determined by a fixed value or expression, rather it is
determined by the current bit number of the message or record it appears in.


attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | mod | pos_integer| | 
 | name | string| | 
 | offset | size| | 


children: none


Without attributes specified, the [pad](#pad) element will consume bits of the
record until the record is byte aligned.  For example, the [pad](#pad) element in following
document will consume 3 bits in order to make the message byte aligned.

    <xddl>
      <field name="A" length="5"/>
      <pad/>
      <field name="B" length="8"/>
    </xddl>
And parsing:

    # idm pad.xddl A014
    
    Name  Length  Value  Hex    Description
    A     5       20     @10100
    pad   3       0      @000
    B     8       20     #14
As we can see, the length of the pad is 3.

If we change the length of the *A* field to 2, we get a pad of 6.

    <xddl>
      <field name="A" length="2"/>
      <pad/>
      <field name="B" length="8"/>
    </xddl>
    # idm pad1.xddl A014
    
    Name  Length  Value  Hex     Description
    A     2       2      @10
    pad   6       32     @100000
    B     8       20     #14
<h2 id="mod">1.14.1 mod</h2>


The *mod* attribute is used to further modify the behavior.  For example,
it may be desireable to pad to the nearest 2-byte boundary, in which case
we would specify a pad of 16.  

<h2 id="peek">1.15 peek</h2>

The [peek](#peek) element provides access to data ahead in the message.  This 
information can then be referenced in expressions.


attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004;  | 
 | length | expression| &#10004;  | 
 | offset | size| &#10004;  | 


children: none


In some protocols a field cannot be decoded correctly until a subsequent
field is known.  The [peek](#peek) element provides a solution for this situation.

    <xddl>
      <peek name="pd" offset="4" length="4"/>
      <switch expr="pd">
        <case value="0">
          <field length="4" name="security header"/>
          <field length="4" name="protocol descriminator"/>
        </case>
        <case value="1">
          <field length="4" name="bearer identity"/>
          <field length="4" name="protocol descriminator"/>
        </case>
       </switch>
    </xddl>
The above example illustrates a typical use of the [peek](#peek) element. Notice the [peek](#peek) "looks ahead" to the "protocol
discriminator" in each of the [case](#case) elements to determine what its value should be.  Then the [switch](#switch) can be properly
evaluated.

<h2 id="prop">1.16 prop</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004;  | 
 | type | url| | 
 | value | expression| | 
 | visible | bool| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="range">1.17 range</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | end | integer| &#10004;  | 
 | href | url| | 
 | value | string| | 
 | start | integer| &#10004;  | 


children: [comment](#comment)

<h2 id="record">1.18 record</h2>
record has multiple attribute signatures

<h2 id="recorddefinition">1.18.1 record definition</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | id | id_url| | 
 | name | string| | 
 | length | expression| | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="recordlink">1.18.2 record link</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| | 
 | href | url| | 
 | length | expression| | 


children: none

<h2 id="repeat">1.19 repeat</h2>
repeat has multiple attribute signatures

<h2 id="repeatindefinitely">1.19.1 repeat indefinitely</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| | 
 | minlen | size| | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="repeatbasedonanexpression">1.19.2 repeat based on an expression</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | num | expression| &#10004;  | 
 | name | string| | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="boundrepeat">1.19.3 bound repeat</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | min | expression| | 
 | max | expression| | 
 | name | string| | 
 | minlen | integer| | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="script">1.20 script</h2>

attributes: none



children: none

<h2 id="setprop">1.21 setprop</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | setprop_name| &#10004;  | 
 | type | url| | 
 | value | expression| &#10004;  | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="start">1.22 start</h2>

attributes: none



children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="switch">1.23 switch</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | expr | expression| &#10004;  | 


children: [case](#case), [default](#default)

<h2 id="type">1.24 type</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | id | id_url| &#10004;  | 
 | name | string| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="uint16">1.25 uint16</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004;  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="uint32">1.26 uint32</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004;  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="uint64">1.27 uint64</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004;  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="uint8">1.28 uint8</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004;  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="while">1.29 while</h2>

attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| | 
 | expr | expression| &#10004;  | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="xddl">1.30 xddl</h2>

attributes: none



children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while), [start](#start), [type](#type), [export](#export)

<h2 id="AttributeTypes">2 Attribute Types</h2>


type | default | description
-----|---------|------------
bool | false |
integer | 0 |
pos_integer | 1 |
size | 0 |
string |  |
expression |  |
setprop_name |  |
url |  |
id_url |  |
jump_name |  |
