#ifndef C_DIGIT
#define C_DIGIT

#include <math.h> 

class cDigit
{
private:
	float value;
	float time;
	float oldV, newV, cTime;
	bool fade;
public:
	cDigit() 
	{
		value = 0; time = -1;
		oldV = newV = cTime = 0;
	};
	cDigit(float val) 
	{
		value = val; time = -1;
		oldV = newV = cTime = 0;
	};
	~cDigit();

	float get() { return value; };
	void setDirect(float val) { value = val; };
	void set(float val)
	{ 
		value = val; 
		time = -1; 
	};
	void set(float val, float time, bool fade = false);
	bool isProgress() { if (time == -1)return false; return true; };
	void update(float dt);

	void set_time(float new_time) { time = new_time; }
	float get_time() { return time; }

	void set_nVal(float new_val) { newV = new_val; }
	float get_nVal() { return newV; }
};


#endif // !C_DIGIT
