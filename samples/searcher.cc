#include <Python.h>

#include <efanna.hpp>
#include <iostream>
#include <fstream>
#include <ctime>
#include <malloc.h>
#include <boost/timer/timer.hpp>

using namespace boost;
using namespace efanna;
using namespace std;

void load_data(char* filename, float*& data, size_t& num,int& dim){// load data with sift10K pattern
  	ifstream in(filename);
	data = (float*)malloc(num * dim * sizeof(float));
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

static PyObject * searcher_search(PyObject *self, PyObject *args){
    char *indata, *intrees, *ingraph, *inquery, *fout, *notr, *it, *psf, *ef, *nn;
    PyArg_ParseTuple(args, "ssssssssss", &indata, &intrees, &ingraph, &inquery, &fout, &notr, &it, &psf, &ef, &nn);

    float* data_load = NULL;
    float* query_load = NULL;
    size_t points_num = 1000000;
    int dim = 100;
    load_data(indata, data_load, points_num,dim);
    size_t q_num = 100;
    int qdim = 100;
    load_data(inquery, query_load, q_num,qdim);
    Matrix<float> dataset(points_num,dim,data_load);
    Matrix<float> query(q_num,qdim,query_load);

    FIndex<float> index(dataset, new L2Distance<float>(), efanna::KDTreeUbIndexParams(true, 8 ,8 ,10,25,30,10));
    index.loadTrees(intrees);
    index.loadGraph(ingraph);

    int search_trees = atoi(notr);
    int search_epoc = atoi(it);
    int poolsz = atoi(psf);
    int search_extend = atoi(ef);
    int search_method = 0;
    index.setSearchParams(search_epoc, poolsz, search_extend, search_trees, search_method);
    boost::timer::auto_cpu_timer timer;
    index.knnSearch(atoi(nn)/*query nn*/,query);
    cout<<timer.elapsed().wall / 1e9<<endl;

    index.saveResults(fout);
    PyObject * ret = PyLong_FromLong(0);
    return ret;
}

static PyMethodDef MyMethods[] = {
    {"search",  searcher_search, METH_VARARGS, "searches"},
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC initsearcher(void){
    (void) Py_InitModule("searcher", MyMethods);
}