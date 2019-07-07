temp_25_331 = custom_target(
    'temp_25_331',
    output : 'temp_25_331.xddl',
    input : '25.331.asn',
    command : [asnx_exe, '-o', '@OUTPUT@', '-t', '@INPUT@'])

x25_331 = custom_target(
    'x25_331',
    output : 'TS-25.331.xddl',
    input : [temp_25_331, 'substitute.txt'],
    build_by_default : true,
    command : [xml_patch_exe, '-o', '@OUTPUT@', '@INPUT0@', '@INPUT1@'])
