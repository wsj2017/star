#!/usr/bin/env python
# -*- coding: UTF-8 -*-

from distutils.core import setup, Extension
import numpy as np

ext_modules = [Extension('atag', 
                        include_dirs = ['/usr/local/include/apriltags'],
                        libraries = ['apriltags'],
                        library_dirs = ['/usr/local/lib'],
                        sources = ['atag_wrap.cpp'])] 

setup(
        name = 'atag',
        version = '1.0',
        include_dirs = [np.get_include()], #Add Include path of numpy
        ext_modules = ext_modules
      )
