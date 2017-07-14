#include "freespace.h"
#include <iostream>
#include <fstream>
#include "input.h"
#include "query.h"
using namespace std;

int main(int argv, char **args)
{ 
  Input input;  
  map <string, Trajectory<double> *> * dataset=input.get_dataset();
  RTree<string, double, 4, double>* se_tree=input.get_se_tree();
  string query_file = "queries.txt";
  bool query_state = true;
  fstream q;
  q.open(query_file.c_str());
  if(q.is_open()){
     while(!q.eof()){
       string line;
       string dataname = "";
       double bound;
       getline(q,line);
       if(line.size()<1){
	  break;
      }
       int i = 0;
       while(line[i] != ' '){
	 dataname = dataname + line[i];
	 i++;
       }
       i = i + 2;
      string bound_str = "";
      while(line[i] != '\n'&&line[i]!='\0'){
	bound_str = bound_str + line[i];
	i++;
      }
      bound = std::stod(bound_str);
      //cout<<bound<<'\n';
      //parallel
      Query query(dataset, dataname, bound, se_tree);
     }
     q.close();
    }
  else{
    //query_state = false;
    cout<<"Error opening query file.";
  }
  return 0;
}


 