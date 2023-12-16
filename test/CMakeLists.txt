configure_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.site.cfg.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/lit.site.cfg.py
  MAIN_CONFIG
  ${CMAKE_CURRENT_SOURCE_DIR}/lit.cfg.py
)

configure_lit_site_cfg(
  ${CMAKE_CURRENT_SOURCE_DIR}/Unit/lit.site.cfg.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/Unit/lit.site.cfg.py
  MAIN_CONFIG
  ${CMAKE_CURRENT_SOURCE_DIR}/Unit/lit.cfg.py
)

list(APPEND VERIF_TEST_DEPS
  verif-static-analyzer
)

add_lit_testsuite(check-verif "Running the verif static analyzer tests"
  ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${VERIF_TEST_DEPS}
)

add_lit_testsuites(VERIF ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${VERIF_TEST_DEPS}
)
