#include <iostream>
#include <sstream>
#include <string>

using std::string;

#include <cstdarg>

#include <png.h>

#define DIMS 3

template <typename T>
class array {
public:
	array<T>(int len) : len(len) {
		d = new T[len];
	}

	~array<T>() {
		delete[] d;
	}

	T &operator[](int p) {
		return d[p];
	}

	int Len() { return len; }

	void foreach(void f(T)) {
		for (int i=0; i<len; i++) {
			f(d[i]);
		}
	}
private:
	T *d;
	int len;
};

template <int D, typename T=double>
class vector {
public:
	vector<D, T>() {
		for (int i=0; i<D; i++)
			d[i] = 0;
	}

	vector<D, T>(void *v, ...) {
		va_list data;
		va_start(data, v);
		for (int i=0; i<D; i++)
			d[i] = va_arg(data, T);
		va_end(data);
	}

	vector<D, T> operator+(vector<D> &v) {
		vector<D> r;
		for (int i=0; i<D; i++)
			r.d[i] = d[i] + v[i];
		return r;
	}

	void operator+=(vector<D> &v) {
		for (int i=0; i<D; i++)
			d[i] += v[i];
	}

	vector<D, T> operator*(double f) {
		vector<D> r;
		for (int i=0; i<D; i++)
			r.d[i] = d[i]*f;
		return r;
	}

	void operator*=(double f) {
		for (int i=0; i<D; i++)
			d[i] *= f;
	}

	string String() {
		std::stringstream o;
		o << "< ";
		for (int i=0; i<D; i++)
			o << d[i] << " ";
		o << ">";
		return o.str();
	}
private:
	T d[D];
};

template <int D>
class ray {
public:

private:
	double freq;
	vector<D> dir;
	vector<D> dest;
};

template <int D>
class camera {
typedef vector<2> vfov;
typedef vector<D> vec;
public:
	camera(vec pos, vfov fov, int resx, int resy)
		: pos(pos), fov(fov), resx(resx), resy(resy) {
		
	}

	array< vector<2> > Pixels() {
		auto rs = array< vector<2> >(resx * resy);
		for (int x=0; x<resx; x++) {
			for (int y=0; y<resy; y++) {
				rs[x * resy + y] = vector<2>(NULL, (double)x, (double)y);
			}
		}
		return rs;
	}
private:
	vec pos;
	vfov fov;
	int resx, resy;
};

int main() {
	camera<DIMS> cam (
		vector<3>(NULL, 0, 0, 0),
		vector<2>(NULL, 0, 0),
		500, 500
	);
	auto pxs = cam.Pixels();
	pxs.foreach(
		[](vector<2> v) {
			std::cout << v.String() << std::endl;
		}
	);
	return 0;
}
