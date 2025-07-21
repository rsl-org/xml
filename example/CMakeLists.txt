project(rsl_xml)

function(DEFINE_EXAMPLE TARGET)
  add_executable(example_${TARGET} "${TARGET}.cpp")
  target_link_libraries(example_${TARGET} PRIVATE rsl_xml)
endfunction()

DEFINE_EXAMPLE(xml)