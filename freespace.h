#ifndef FREESPACE
#define FREESPACE


#include "trajectory.h"
#include <vector>
#include "point.h"
#include "cell.h"
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <string>

using namespace std;
/*
template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct Cell_position  
{  
    int x;  
    int y;  
    Cell_position(int x, int y)  
    {  
        this->x = x;  
        this->y = y;  
    }  
  
    bool operator== (const Cell_position & c) const  
    {  
        return x==c.x && y==c.y;  
    }  
};  

size_t hash_value(const int i)  
{  
    size_t seed = 0;  
    hash_combine(seed, i);  
    return seed;  
}  

size_t hash_value(const Cell_position & c)  
{  
    size_t seed = 0;  
    hash_combine(seed, hash_value(c.x));  
    hash_combine(seed, hash_value(c.y));  
    return seed;  
} */ 
 
template <typename T>
class Freespace
{
private:
  list < Cell<T> > cell_array;
  bool result;
  Trajectory<T> * queried;
  int queried_size;
  Trajectory<T> * input;
  int input_size;
  T bound;
  unordered_map<string,Cell<T> > black_cell;
  //map < Point<int> , Cell<T> > black_cell;
  double min(double a, double b){
    if (a < b)
      return a;
    else
      return b;
  }
  
  double max(double a, double b){
    if (a > b)
      return a;
    else
      return b;
  }
  
public:
  Freespace(Trajectory<T>* q, Trajectory<T>* i, T b, int qs, int is){
    queried=q;
    queried_size=qs;
    bound=b;
    input=i;
    input_size=is;
    result=false;
  }
  ~Freespace(){
    cell_array.clear();
  }
  bool algorithm(){
     int t=0;
    Cell<T> start_cell(queried -> get_point(0), queried -> get_point(1), 
		    input -> get_point(0), input -> get_point(1), 1,1, bound);
    start_cell.calculate_LF();
    start_cell.calculate_BF();
    start_cell.calculate_UF();
    start_cell.calculate_RF();
    if(!(start_cell.get_LF1()==0||start_cell.get_BF1()==0)){
      //cout<<"hh\n";
      return result;
    }
    start_cell.calculate_UR_start();
    start_cell.calculate_RR_start();
    cell_array.push_back(start_cell);
    while(cell_array.size() > 0){
      t++;
      int size = cell_array.size();
      //cout<<"("<<cell_array.back().get_s_index()<<","
      //<<cell_array.back().get_t_index()<<","<<size<<")";
      if(cell_array.back().get_s_index() == queried_size-1 && 
	 cell_array.back().get_t_index() == input_size-1&&
         (cell_array.back().get_RR2()==1||cell_array.back().get_UR2()==1)){
	result=true;
	break;
	}
	int s=cell_array.back().get_s_index();
	int t=cell_array.back().get_t_index();
      if(cell_array.back().get_test_up() == false){
	cell_array.back().set_test_up_true();
	if(cell_array.back().up_reach()){
	  //cout<<cell_array[size-1].get_t_index()<<" t \n";
	  if(cell_array.back().get_t_index()<input_size-1){
	    //test whether up_cell is black
	    //const Point<int> st_index(s,t+1);
	    string str1 = std::to_string(s);
	    string str2 = std::to_string(t+1);
	    string str_result="x"+str1+"y"+str2;
	    if(black_cell.find(str_result)==black_cell.end()){ 
	      Cell<T> newcell(queried -> get_point(s-1), 
			    queried -> get_point(s), 
			    input ->  get_point(t), 
			    input -> get_point(t+1), 
			    s, t+1, bound);
	      newcell.set_BR(cell_array.back().get_UR1(),
			       cell_array.back().get_UR2());
	      newcell.calculate_UR_up();
	      newcell.calculate_RR_up();
	      cell_array.push_back(newcell);
	    }
	    else{
	     Cell<T> old_cell= black_cell.find(str_result)->second;
	     double oldUR1 = old_cell.get_UR1();
	     double oldUR2 = old_cell.get_UR2(); 
	     double oldRR1 = old_cell.get_RR1(); 
	     double oldRR2 = old_cell.get_RR2(); 
	     
	     double newUR1=-1;
	     double newUR2=-1;
	     double newRR1=-1;
	     double newRR2=-1;
	     double newBR1=cell_array.back().get_UR1();
	     bool up_update=false;
	     bool right_update=false;
	     
	     if(old_cell.get_UF2() >= newBR1 && newBR1!=-1){
	       newUR1 = max(old_cell.get_UF1(), newBR1);
	       newUR2 = old_cell.get_UF2();
	     }
	     newRR1 = old_cell.get_RF1();
	     newRR2 = old_cell.get_RF2();
	     
	     if(!(newUR1==-1||newUR2==-1)){
	       if(oldUR1 > newUR1 || oldUR2 < newUR2 ){
		 up_update=true;
	       }
	     }
	     
	     if(!(newRR1==-1||newRR2==-1)){
	       if(oldRR1 > newRR1 || oldRR2 < newRR2 ){
		 right_update=true;
	       }
	     }
	     
	     if(up_update==true||right_update==true){
	       black_cell.erase(str_result);
	       old_cell.set_UR(newUR1, newUR2);
	       old_cell.set_RR(newRR1, newRR2);
	       cell_array.push_back(old_cell);
	     }
	    }
	  }
	}
      }
      else if(cell_array.back().get_test_right() == false){
	cell_array.back().set_test_right_true();
	if(cell_array.back().right_reach()){
	  if(cell_array.back().get_s_index()<queried_size-1){	    
	    //const Point<int> st_index(s+1,t);
	    string str1 = std::to_string(s+1);
	    string str2 = std::to_string(t);
	    string str_result="x"+str1+"y"+str2;
	    if(black_cell.find(str_result)==black_cell.end()){ 
	      Cell<T> newcell(queried -> get_point(s), 
			    queried -> get_point(s+1), 
			    input ->  get_point(t-1), 
			    input -> get_point(t), 
			    s+1, t, bound);
	      newcell.set_LR(cell_array.back().get_RR1(),
			       cell_array.back().get_RR2());
	      newcell.calculate_UR_right();
	      newcell.calculate_RR_right();
	      cell_array.push_back(newcell);
	      //cout<<'\n';
	    }
	   else{
	     Cell<T> old_cell= black_cell.find(str_result)->second;
	     double oldUR1 = old_cell.get_UR1();
	     double oldUR2 = old_cell.get_UR2(); 
	     double oldRR1 = old_cell.get_RR1(); 
	     double oldRR2 = old_cell.get_RR2(); 
	     
	     double newUR1=-1;
	     double newUR2=-1;
	     double newRR1=-1;
	     double newRR2=-1;
	     double newLR1=cell_array.back().get_RR1();
	     bool up_update=false;
	     bool right_update=false;
	     
	     if(old_cell.get_RF2() >= newLR1 && newLR1!=-1){
	       newRR1 = max(old_cell.get_RF1(), newLR1);
	       newRR2 = old_cell.get_UF2();
	     }
	     newUR1 = old_cell.get_UF1();
	     newUR2 = old_cell.get_UF2();
	     
	     if(!(newUR1==-1||newUR2==-1)){
	       if(oldUR1 > newUR1 || oldUR2 < newUR2 ){
		 up_update=true;
	       }
	     }
	     
	     if(!(newRR1==-1||newRR2==-1)){
	       if(oldRR1 > newRR1 || oldRR2 < newRR2 ){
		 right_update=true;
	       }
	     }
	     
	     if(up_update==true||right_update==true){
	       black_cell.erase(str_result);
	       old_cell.set_UR(newUR1, newUR2);
	       old_cell.set_RR(newRR1, newRR2);
	       cell_array.push_back(old_cell);
	     }
	    }
	   }
	  }
	}
      else{
	//const Point<int> st_index(s,t);
	    string str1 = std::to_string(s);
	    string str2 = std::to_string(t);
	    string str_result="x"+str1+"y"+str2;
	if(black_cell.find(str_result)==black_cell.end()){
	 black_cell.insert(std::pair<string,Cell<T> >
	 (str_result,cell_array.back())); 
	 cell_array.pop_back();
	}
	else{
	  cout<<"Error";
	  result=false;
	  break;
	}
      }
    }
    return result;
  }
};

#endif
