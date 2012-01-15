#include <iostream>
using namespace std;

#include <png.h>

template <int D>
class Vector {
public:
	Vector<D>() {
		for (int i=0; i<D; i++)
			d[i] = 0;
	}

	Vector<D> operator+(Vector<D> &v) {
		Vector<D> r;
		for (int i=0; i<D; i++)
			r.d[i] = d[i] + v[i];
		return r;
	}

	void operator+=(Vector<D> &v) {
		for (int i=0; i<D; i++)
			d[i] += v[i];
	}

	Vector<D> operator*(double f) {
		Vector<D> r;
		for (int i=0; i<D; i++)
			r.d[i] = d[i]*f;
		return r;
	}

	void operator*=(double f) {
		for (int i=0; i<D; i++)
			d[i] *= f;
	}
private:
	double d[D];
};

typedef Vector<3> Vec;

template <int D>
class Ray {
public:

private:
	double freq;
	Vector<D> dir;
	Vector<D> dest;
};

template <int D>
class Camera {
public:

private:
	
};

int main() {
	Vec v;
	return 0;
}
