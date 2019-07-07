temp_36_331 = custom_target(
    'temp_36_331',
    output : 'temp.xddl',
    input : '36.331.asn',
    command : [asnx_exe, '-o', '@OUTPUT@', '-t', '@INPUT@'])

x36_331 = custom_target(
    'x36_331',
    output : 'TS-36.331.xddl',
    input : [temp_36_331, 'substitute.txt'],
    build_by_default : true,
    command : [xml_patch_exe, '-o', '@OUTPUT@', '@INPUT0@', '@INPUT1@'])

