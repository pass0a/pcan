#cmake file for project association
#author: liuwenjun
#created:2014/02/20

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

## 添加项目

if(MSVC)
LINK_DIRECTORIES (${PROJECT_SOURCE_DIR}/lib)
add_library(pcan SHARED
				${PROJECT_SOURCE_DIR}/binding/index.cpp
				${PROJECT_SOURCE_DIR}/binding/jsPcan.cpp
				${PROJECT_SOURCE_DIR}/binding/export.def
				${PROJECT_SOURCE_DIR}/src/pcan.cpp

				)
target_link_libraries(pcan
             PCANBasic
             )
set(CMAKE_SHARED_LIBRARY_SUFFIX ".passoa")
install(TARGETS pcan
	RUNTIME DESTINATION ./node_modules/pcan)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/modules/ DESTINATION node_modules/pcan)
install(FILES ${PROJECT_SOURCE_DIR}/lib/PCANBasic.dll DESTINATION node_modules/pcan)
else()

endif()
