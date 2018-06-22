#include <Python.h>

#include <efanna.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <malloc.h>
#include <boost/timer/timer.hpp>
using namespace efanna;
using namespace std;

void load_data(char* filename, float*& data, size_t& num,int& dim){// load data with sift10K pattern
  	ifstream in(filename);
	int i = 0;
	std::string str;
	int lines = 0;
	while(std::getline(in, str)){
		istringstream iss(str);
		do{
        	string subs;
        	iss >> subs;
			float t = strtof((subs).c_str(),0);
    		if(subs == ""){
				break;
			}else{
				data[i] = t;
				i++;
			}
    	}while(iss);
		lines ++;
	}
  	in.close();
}

static PyObject * builder_build_graph(PyObject *self, PyObject *args){
    char *fin, *fout, *notr, *ctd, *it, *cL, *c, *K, *cS;
    //int notr, ctd, it, L, c, K, S;
    PyArg_ParseTuple(args, "sssssssss", &fin, &fout, &notr, &ctd, &it, &cL, &c, &K, &cS);
    //cout<<"fin "<<fin<<" fout "<<fout<<" notr "<<notr<<" ctd "<<ctd<<" it "<<it<<" L "<<L<<" c "<<c<<" K "<<K<<" S "<<S<<endl;

    float* data;// = NULL;
  	//float* query_load = NULL;
  	size_t points_num = 1000000;
  	int dim = 100;
	data = (float*)malloc(points_num * dim * sizeof(float));
	load_data(fin, data, points_num, dim);
	Matrix<float> dataset(points_num, dim, data);
  	//Matrix<float> query(q_num,qdim,query_load);

  	unsigned int trees = atoi(notr);
  	int mlevel = atoi(ctd);
  	unsigned int epochs = atoi(it);
  	int L = atoi(cL);
  	int checkK = atoi(c);
  	int kNN = atoi(K);
  	int S = atoi(cS);

  	FIndex<float> index(dataset, new L2Distance<float>(), efanna::KDTreeUbIndexParams(true, trees ,mlevel ,epochs,checkK,L, kNN, trees, S));
	boost::timer::auto_cpu_timer timer;
	index.buildIndex();
	cout<<timer.elapsed().wall / 1e9<<endl;
  	index.saveGraph(fout);
    PyObject * ret = PyLong_FromLong(0);
    return ret;
}

static PyObject * builder_build_trees(PyObject *self, PyObject *args){
    char *fin, *fout, *notr;
    PyArg_ParseTuple(args, "sss", &fin, &fout, &notr);
    //cout<<"fin "<<fin<<" fout "<<fout<<" notr "<<notr<<endl;

    float* data;
  	//float* query_load = NULL;
  	size_t points_num = 1000000;
  	int dim = 100;
	data = (float*)malloc(points_num * dim * sizeof(float));
  	load_data(fin, data, points_num, dim);
	Matrix<float> dataset(points_num,dim,data);

  	unsigned int trees = atoi(notr);

  	FIndex<float> index(dataset, new L2Distance<float>(), efanna::KDTreeUbIndexParams(true, trees ,8 ,8,25,30, 10, 8, 10));
  	clock_t s,f;
  	s = clock();
  	index.buildTrees();

  	f = clock();
  	cout<<"Index building time : "<<(f-s)*1.0/CLOCKS_PER_SEC<<" seconds"<<endl;
  	index.saveTrees(fout);

    PyObject * ret = PyLong_FromLong(0);
    return ret;
}

static PyMethodDef MyMethods[] = {
    {"build_trees",  builder_build_trees, METH_VARARGS, "builds trees"},
    {"build_graph",  builder_build_graph, METH_VARARGS, "builds graph"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};


/*DL_EXPORT(void) initdemo(void){
    Py_InitModule("demo", MyMethods);
}*/

PyMODINIT_FUNC initbuilder(void){
    //(void) Py_InitModule3("demo", MyMethods, "hi");
    (void) Py_InitModule("builder", MyMethods);
}
/*PyMODINIT__FUNC PyInit_buildgraph(void){
    PyModule_Create(&py_buildgraph);
}

static struct PyModuleDef buildmodule = {
    PyModuleDef_HEAD_INIT,
    "buildgraph",
    NULL,
    -1,
    MyMethods
};

PyMODINIT_FUNC
PyInit_buildgraph(void)
{
    return PyModule_Create(&buildgraph_run);
}*/