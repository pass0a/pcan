#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目
# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})

add_library(pcan SHARED
				index.cpp
				pcan.cpp
				)
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR} ${OpenCV_INCLUDE_DIRS} ${CMAKE_JS_INC} ${PCAN_DIR}/Include)
target_link_libraries(pcan
				${CMAKE_JS_LIB}
				${PCAN_DIR}/x64/VC_LIB/PCANBasic.lib
			 )
			 message("Test:" ${PCAN_DIR}/x64/VC_LIB/PCANBasic.lib)
set(CMAKE_SHARED_LIBRARY_SUFFIX ".node")
add_custom_command(TARGET pcan POST_BUILD 
                   COMMAND ${CMAKE_COMMAND} -E copy ${PCAN_DIR}/x64/PCANBasic.dll ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

