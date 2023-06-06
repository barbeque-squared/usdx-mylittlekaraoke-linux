CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )
PROJECT( mlkhs-linux )

# in our case: linux
#Show all warnings and make it c++11
set( CMAKE_CXX_FLAGS "-std=c++17 -g -Wall" )

#~ set(CURL_LIBRARY "-lcurl")
find_package(CURL)

ADD_LIBRARY( mlkhs-linux SHARED mlkhs-linux.cpp )
target_link_libraries(mlkhs-linux ${CURL_LIBRARIES})
