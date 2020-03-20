#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目

if(MSVC)
LINK_DIRECTORIES (${PCAN_LIB})
add_library(pcan SHARED
				index.cpp
				jsPcan.cpp
				export.def
				pcan.cpp
				)
target_link_libraries(pcan
             PCANBasic
             )
set(CMAKE_SHARED_LIBRARY_SUFFIX ".node")
add_custom_command(TARGET pcan POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy ${PCAN_LIB}/PCANBasic.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else()

endif()
