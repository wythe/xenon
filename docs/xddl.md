#  XDDL Element Reference
* 1 [Elements ](#Elements)
    * 1.1 [bit ](#bit)
    * 1.2 [case ](#case)
    * 1.3 [comment ](#comment)
    * 1.4 [cstr ](#cstr)
    * 1.5 [default ](#default)
    * 1.6 [enc ](#enc)
    * 1.7 [export ](#export)
    * 1.8 [field ](#field)
    * 1.9 [fragmen ](#fragmen)
    * 1.10 [if ](#if)
    * 1.11 [item ](#item)
    * 1.12 [jump ](#jump)
    * 1.13 [oob ](#oob)
    * 1.14 [pad ](#pad)
    * 1.15 [peek ](#peek)
    * 1.16 [prop ](#prop)
    * 1.17 [range ](#range)
    * 1.18 [record ](#record)
        * 1.18.1 [basic record](#basicrecord)
        * 1.18.2 [record link](#recordlink)
        * 1.18.3 [record definition](#recorddefinition)
    * 1.19 [repeat ](#repeat)
        * 1.19.1 [repeat indefinitely](#repeatindefinitely)
        * 1.19.2 [repeat based on an expression](#repeatbasedonanexpression)
        * 1.19.3 [bound repeat](#boundrepeat)
    * 1.20 [script ](#script)
    * 1.21 [setprop ](#setprop)
    * 1.22 [start ](#start)
    * 1.23 [switch ](#switch)
    * 1.24 [type ](#type)
    * 1.25 [uint16 ](#uint16)
    * 1.26 [uint32 ](#uint32)
    * 1.27 [uint64 ](#uint64)
    * 1.28 [uint8 ](#uint8)
    * 1.29 [while ](#while)
    * 1.30 [xddl ](#xddl)
* 2 [Attribute Types ](#AttributeTypes)

<h2 id="Elements">1 Elements </h2>
<h2 id="bit">1.1 bit </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="case">1.2 case </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | value | integer| &#10004  | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="comment">1.3 comment </h2>


attributes: none



children: none

<h2 id="cstr">1.4 cstr </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | max | expression| | 
 | name | string| &#10004  | 


children: none

<h2 id="default">1.5 default </h2>


attributes: none



children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="enc">1.6 enc </h2>


attributes: none



children: [fragment](#fragment), [type](#type), [start](#start), [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="export">1.7 export </h2>


attributes: none



children: [prop](#prop)

<h2 id="field">1.8 field </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004  | 
 | type | url| | 
 | bias | integer| | 
 | length | expression| &#10004  | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="fragmen">1.9 fragmen </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | href | url| &#10004  | 


children: none

<h2 id="if">1.10 if </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | expr | expression| &#10004  | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="item">1.11 item </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | key | integer| &#10004  | 
 | href | url| | 
 | value | string| &#10004  | 


children: [comment](#comment)

<h2 id="jump">1.12 jump </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | base | jump_name| &#10004  | 


children: none

<h2 id="oob">1.13 oob </h2>


attributes: none



children: [type](#type), [start](#start), [export](#export), [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="pad">1.14 pad </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | mod | pos_integer| | 
 | name | string| | 
 | offset | size| | 


children: none

<h2 id="peek">1.15 peek </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004  | 
 | length | expression| &#10004  | 
 | offset | size| &#10004  | 


children: none

<h2 id="prop">1.16 prop </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004  | 
 | type | url| | 
 | value | expression| | 
 | visible | bool| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="range">1.17 range </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | end | integer| &#10004  | 
 | href | url| | 
 | value | string| | 
 | start | integer| &#10004  | 


children: [comment](#comment)

<h2 id="record">1.18 record </h2>


record has multiple attribute signatures

<h2 id="basicrecord">1.18.1 basic record</h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
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

<h2 id="recorddefinition">1.18.3 record definition</h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | id | id_url| | 
 | name | string| | 
 | length | expression| | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="repeat">1.19 repeat </h2>


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
 | num | expression| &#10004  | 
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

<h2 id="script">1.20 script </h2>


attributes: none



children: none

<h2 id="setprop">1.21 setprop </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | setprop_name| &#10004  | 
 | type | url| | 
 | value | expression| &#10004  | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="start">1.22 start </h2>


attributes: none



children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="switch">1.23 switch </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | expr | expression| &#10004  | 


children: [case](#case), [default](#default)

<h2 id="type">1.24 type </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | id | id_url| &#10004  | 
 | name | string| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="uint16">1.25 uint16 </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="uint32">1.26 uint32 </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="uint64">1.27 uint64 </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="uint8">1.28 uint8 </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| &#10004  | 
 | type | url| | 
 | bias | integer| | 
 | default | expression| | 


children: [comment](#comment), [item](#item), [range](#range), [script](#script)

<h2 id="while">1.29 while </h2>




attributes | name  | [type](#AttributeTypes) | required | description
-----------|-------|--------|----------|------------
 | name | string| | 
 | expr | expression| &#10004  | 


children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while)

<h2 id="xddl">1.30 xddl </h2>


attributes: none



children: [bit](#bit), [comment](#comment), [cstr](#cstr), [field](#field), [fragment](#fragment), [jump](#jump), [if](#if), [oob](#oob), [pad](#pad), [peek](#peek), [enc](#enc), [prop](#prop), [record](#record), [repeat](#repeat), [setprop](#setprop), [switch](#switch), [uint8](#uint8), [uint16](#uint16), [uint32](#uint32), [uint64](#uint64), [while](#while), [start](#start), [type](#type), [export](#export)

<h2 id="AttributeTypes">2 Attribute Types </h2>


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
