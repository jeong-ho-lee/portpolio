#pragma once

template<typename T>
class cclass {
private:
	T temp;
public:
	cclass() : temp(0) {}
	T GetTemp();
};