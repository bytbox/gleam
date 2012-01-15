#include <iostream>
#include <sstream>
#include <string>
#include <utility>

using std::string;

#include <cstdarg>

#include <png.h>

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
		for (int i=0; i<len; i++)
			f(d[i]);
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

	vector<D, T>(std::initializer_list<T> l) {
		auto it = l.begin();
		for (int i=0; i<D; i++)
			if (it != l.end()) d[i] = *(it++);
			else d[i] = 0;
	}

	T &operator[](int p) {
		return d[p];
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

	string String() const {
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

template <int D, typename T=double>
std::ostream &operator<<(std::ostream &os, const vector<D, T> &v) {
	os << v.String();
	return os;
}

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

	array<vector<2>> Pixels() {
		auto rs = array<vector<2>>(resx * resy);
		for (int x=0; x<resx; x++) {
			for (int y=0; y<resy; y++) {
				rs[x * resy + y] = vector<2>{(double)x, (double)y};
			}
		}
		return rs;
	}

	array<vec> Destinations() {
		auto pxs = Pixels();
		auto r = array<vector<D>>(pxs.Len());
		for (int i=0; i<r.Len(); i++) {
			auto v = pxs[i];
			r[i] = vector<D>{v[0], v[1]};
			r[i] += pos;
		}
		return r;
	}
private:
	vec pos;
	vfov fov;
	int resx, resy;
};

int main() {
	camera<DIMS> cam (
		vector<DIMS>{-4, -6, -8},
		vector<2>{0, 0},
		10, 10
	);

	cam.Destinations().foreach(
		[](vector<DIMS> v) {
			std::cout << v << std::endl;
		}
	);
	return 0;
}
