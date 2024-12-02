cmake_minimum_required(VERSION 3.5.0)
project(
  Structures_lab_5
  VERSION 0.1.0
  LANGUAGES C)

set(EXECUTABLE_NAME sorts)
add_executable(${EXECUTABLE_NAME} sorts.c)

include(CTest)
cmake_policy(SET CMP0110 NEW)
enable_testing()

set(RUNNER test_runner.py)
configure_file(${RUNNER} test_runner.py COPYONLY)

set(MODES 1; 2; 3)
set(COUNTS 5; 10; 100)
set(LOW_LIMITS 0; -1000)
set(HIGH_LIMITS 10; 1000)

foreach(mode ${MODES})
  foreach(count ${COUNTS})
    foreach(low_limit ${LOW_LIMITS})
      foreach(high_limit ${HIGH_LIMITS})
        add_test(
          NAME "Mode ${mode}, ${count} numbers in range ${low_limit}:${high_limit}"
          COMMAND
            python3 test_runner.py
            "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME} ${mode} ${count} ${low_limit} ${high_limit}"
        )
      endforeach(high_limit ${HIGH_LIMITS})
    endforeach(low_limit ${LOW_LIMITS})
  endforeach(count ${COUNTS})
endforeach(mode MODES)
