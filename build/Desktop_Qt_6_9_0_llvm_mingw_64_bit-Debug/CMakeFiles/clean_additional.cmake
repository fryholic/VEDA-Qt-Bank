# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\VEDA-Qt-BANK_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\VEDA-Qt-BANK_autogen.dir\\ParseCache.txt"
  "VEDA-Qt-BANK_autogen"
  )
endif()
