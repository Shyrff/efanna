original repo https://github.com/ZJULearning/efanna

to customize  input format/usage vector operation edit builder.cc in samples dir. If you want to use vector operation edit matrix constructor to use it in general/matrix.hpp too. After making all changes run 'python builder.py build', find new module located in samples/build/ folder, move it to your python project location and now you can import and use it.

API:
builder.buid_trees() and builder.build_graph() take the same args and do the same as efanna_index_buildgraph and efanna_index_buildtrees from samples dir
