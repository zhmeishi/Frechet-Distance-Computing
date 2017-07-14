#include "freespace.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include "input.h"
#include "query.h"
using namespace std;

int main(int argv, char **args)
{	time_t before_input;
	time(&before_input);
	if(argv==3){
		string query_file = args[2];
		Input input(args[1]);
	}
	else if(argv > 3 || argv == 2){
		cout<<"Error Argument. Please view Readme.\n";
		return 0;
	}
	else{
		Input input();
		string query_file = "queries.txt";
	} 
	time_t after_input;
	time(&after_input); 
	double seconds_i = difftime(after_input,before_input);
	printf ("%.f seconds for load data.\n", seconds_i);
	map <string, Trajectory<double> *> * dataset=input.get_dataset();
	RTree<string, double, 4, double>* se_tree=input.get_se_tree();
	bool query_state = true;
	fstream q;
	time_t before_query;
	time(&before_query);
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
			while(line[i] = ' '){
				i++;
			}
			
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
		time_t after_query;
		time(&after_query); 
		double seconds_q = difftime(after_query,before_query);
		printf ("%.f seconds for load data.\n", seconds_q);
	}
	else{
		//query_state = false;
		cout<<"Error opening query file.\n";
	}
	return 0;
}



