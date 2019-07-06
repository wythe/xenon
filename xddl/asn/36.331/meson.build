# add_custom_command(['OUTPUT', cmake_current_source_dir, '/../../3GPP/TS-36.331.xddl', 'DEPENDS', '36.331.asn', 'substitute.txt', 'asnx', 'xml-patch', 'COMMAND', 'asnx', '-t', cmake_current_source_dir, '/36.331.asn', '>', 'temp.xddl', 'COMMAND', 'xml-patch', 'temp.xddl', cmake_current_source_dir, '/substitute.txt', '>', cmake_current_source_dir, '/../../3GPP/TS-36.331.xddl'])
# add_custom_target(['36_331', 'ALL', 'DEPENDS', cmake_current_source_dir, '/../../3GPP/TS-36.331.xddl'])
#

temp_xddl = custom_target(
    'temp_xddl',
    output : 'temp.xddl',
    input : '36.331.asn',
    command : [asnx_exe, '-t', '@INPUT@', '>', '@OUTPUT@'])

x36_331 = custom_target(
    'x36_331',
    output : 'TS-36.331.xddl',
    input : [temp_xddl, 'substitute.txt'],
    build_by_default : true,
    command : [xml_patch_exe, '@INPUT0@', '@INPUT1@', '>', '@OUTPUT'])

