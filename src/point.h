#ifndef POINT
#define POINT


template <class T>
class Point{
	public:
		T x_coo;
		T y_coo;
		Point(T x,T y){x_coo=x; y_coo=y;}
		Point(){}
		//int number;
		//int t_id;
		bool operator==(const Point<T> &p) const
		{
			return x_coo==p.x_coo && y_coo==p.y_coo;
		}
		bool operator< (const Point<T> &p) const
		{
			return x_coo<p.x_coo || (x_coo==p.x_coo && y_coo<p.y_coo);
		}

};
/*
   namespace std {
   struct hash<Point<int> > {
   std::size_t operator()(const Point& p) const {
   using std::size_t;
   using std::hash;

// Compute individual hash values for first,
// second and third and combine them using XOR
// and bit shifting:
return ((hash<int>()(p.x_coo)
^ (hash<int>()(p.ycoo) << 1)) >> 1);
}
};*/

#endif
