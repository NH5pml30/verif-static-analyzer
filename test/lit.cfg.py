import lit.formats

config.name = 'Verif-Static-Analyzer'
config.test_format = lit.formats.ShTest(True)

config.suffixes = ['.c', '.cpp', '.cc']

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = os.path.join(config.my_obj_root, 'test')

config.substitutions.append(('%verif',
    os.path.join(config.my_obj_root, 'bin', 'verif-static-analyzer')))
config.substitutions.append(('%FileCheck', config.my_filecheck))
