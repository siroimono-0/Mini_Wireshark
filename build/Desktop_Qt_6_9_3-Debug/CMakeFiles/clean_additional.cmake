# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/appMini_Wireshark_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/appMini_Wireshark_autogen.dir/ParseCache.txt"
  "appMini_Wireshark_autogen"
  )
endif()
