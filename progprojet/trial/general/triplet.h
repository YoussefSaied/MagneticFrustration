#include <vector>
#include <cmath>

class triplet{

public:
	triplet(int x, int y, int z):x(x), y(y), z(z)  {}
	int x;
	int y;
	int z;
	bool operator<(triplet t) const{
		if(			t.x-x <= 	1
				and t.y-y 	<=	1
				and t.z-z 	<=	1) return true;
				return false;
	}
};
