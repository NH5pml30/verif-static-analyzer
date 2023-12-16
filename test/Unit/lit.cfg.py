import lit.formats

config.name = 'Verif-Static-Analyzer-Unit'
config.test_format = lit.formats.GoogleTest(".", "Tests")

config.suffixes = []

config.test_exec_root = os.path.join(config.my_obj_root, 'unittests')
config.test_source_root = config.test_exec_root
