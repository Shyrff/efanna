from distutils.core import setup, Extension

module1 = Extension('builder',
                    include_dirs=['/home/maxim/PycharmProjects/EFANNAbinding/', '/home/maxim/PycharmProjects/EFANNAbinding/general/', '/home/maxim/PycharmProjects/EFANNAbinding/algorithm/'],
                    extra_compile_args=['-std=c++11', '-march=native','-fopenmp'],
                    extra_link_args=['-lgomp', '-lboost_timer', '-lboost_chrono', '-lboost_system'],
                    sources = ['builder.cc'])

setup(name = 'builder', version = 0.1, description = 'builder for efannapy', ext_modules = [module1])