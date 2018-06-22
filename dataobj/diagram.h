#ifndef _diagram_h_
#define _diagram_h_
#include "../simline.h"
#include "../tpl/vector_tpl.h"

class diagram_t
{
public:
	diagram_t();
	~diagram_t();
	void append_diagram_entry(char* station_name, int total_distance, bool is_reverse);
	void modify_station_name_at(int i, char* station_name){
		station_name_list[i]=station_name;
	}
	void modify_distance_at(int i, int total_distance){
		total_distance_list[i]=total_distance;
	}
	void set_diagram_entries_from(linehandle_t line);
	vector_tpl<const char*> get_station_name_list() const {return station_name_list;}
	vector_tpl<int> get_total_distance_list() const {return total_distance_list;}
private:
	vector_tpl<const char*> station_name_list;// unduplicated station name list (vector)
	vector_tpl<int> stop_order_list;//the order of stop.
	vector_tpl<bool> is_reverse_list;//reversing->1, passing->0
	vector_tpl<int> total_distance_list;//in meter, able to access using stop_order_list
	bool is_reverse_line;
};
#endif
