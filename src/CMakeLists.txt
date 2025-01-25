# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.10)

project(sensor_measures_hr_flutter_package_library VERSION 0.0.1 LANGUAGES C)

add_library(sensor_measures_hr_flutter_package SHARED
  "sensor_measures_hr_flutter_package.c"
)

set_target_properties(sensor_measures_hr_flutter_package PROPERTIES
  PUBLIC_HEADER sensor_measures_hr_flutter_package.h
  OUTPUT_NAME "sensor_measures_hr_flutter_package"
)

target_compile_definitions(sensor_measures_hr_flutter_package PUBLIC DART_SHARED_LIB)
