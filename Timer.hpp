//Copyright (c) 2013, Dmitri V. Kalashnikov. All rights reserved.
//This copyright notice should remain at the top of this file.
//

#ifndef TIMER_HPP
#define TIMER_HPP

class Timer
{
private:
	double start_time;

public:
	Timer();

	void start();
	double getTime();

	static void print();
};

#endif
