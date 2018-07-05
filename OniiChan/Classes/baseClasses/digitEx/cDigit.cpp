#include "cDigit.hpp"


cDigit::~cDigit()
{
}

void cDigit::set(float val, float t, bool f)
{
	oldV = value;
	newV = val;
	time = t;
	cTime = t;
	fade = f;
}

void cDigit::update(float dt)
{
	if (!fade)
	{
		if (time < 0 && time != -1)
		{
			value = newV;
			time = -1;
		}
		if (time != -1)
		{
			float proc = 100.0f - time / (cTime / 100.0f);
			value = oldV + ((newV - oldV) / 100.0f) * proc;

			time -= dt;
		}
	}
	else
	{
		if (time < 0 && time != -1)
		{
			value = newV;
			time = -1;
		}
		if (time != -1)
		{
			float timeProc = 100.0f - (cTime - time) / (cTime / 100.0f);
			float x = (10.0f / 100.0f) * timeProc;
			float y = 100.0f - (x * x);
			value = oldV - ((oldV - newV) / 100.0f) * y;
			time -= dt;
		}
	}
}

