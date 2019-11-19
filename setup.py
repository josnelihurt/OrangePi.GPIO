from distutils.core import setup, Extension

module1 = Extension('pca9685', sources=['src/py_lib.c', 'src/PCA9685.c'])

setup(name='pca9685',
      version='1.0',
      description='This is a demo package',
      ext_modules=[module1])
