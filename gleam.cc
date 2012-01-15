#include <iostream>
#include <sstream>
#include <string>
#include <utility>

using std::string;

#include <cstdarg>

#include <png.h>

template <typename T>
class iterable {
public:
	virtual uint64_t Len() = 0;
	virtual T &operator[](uint64_t) = 0;

	virtual void foreach(void f(T)) {
		for (uint64_t i=0; i<Len(); i++)
			f((*this)[i]);
	}
};

template <typename T>
class array : public iterable<T> {
public:
	array<T>(uint64_t len) : len(len) {
		d = new T[len];
	}

	~array<T>() {
		delete[] d;
	}

	T &operator[](uint64_t p) {
		return d[p];
	}

	virtual uint64_t Len() { return len; }

private:
	T *d;
	uint64_t len;
};

template <uint64_t D, typename T=double>
class vector : public iterable<T> {
public:
	vector<D, T>() {
		for (uint64_t i=0; i<D; i++)
			d[i] = 0;
	}

	vector<D, T>(T e) {
		for (uint64_t i=0; i<D; i++)
			d[i] = e;
	}

	vector<D, T>(std::initializer_list<T> l) {
		auto it = l.begin();
		for (uint64_t i=0; i<D; i++)
			if (it != l.end()) d[i] = *(it++);
			else d[i] = 0;
	}

	T &operator[](uint64_t p) {
		return d[p];
	}

	virtual uint64_t Len() { return D; }

	vector<D, T> operator+(vector<D> &v) {
		vector<D> r;
		for (uint64_t i=0; i<D; i++)
			r.d[i] = d[i] + v[i];
		return r;
	}

	void operator+=(vector<D> &v) {
		for (uint64_t i=0; i<D; i++)
			d[i] += v[i];
	}

	vector<D, T> operator*(double f) {
		vector<D> r;
		for (uint64_t i=0; i<D; i++)
			r.d[i] = d[i]*f;
		return r;
	}

	void operator*=(double f) {
		for (uint64_t i=0; i<D; i++)
			d[i] *= f;
	}

	string String() const {
		std::stringstream o;
		o << "< ";
		for (uint64_t i=0; i<D; i++)
			o << d[i] << " ";
		o << ">";
		return o.str();
	}
private:
	T d[D];
};

template <uint64_t D, typename T=double>
std::ostream &operator<<(std::ostream &os, const vector<D, T> &v) {
	os << v.String();
	return os;
}

template <uint64_t D>
class ray {
public:
	ray<D>() {}
	ray<D>(vector<D> dest, vector<D> dir) : dest(dest), dir(dir) {}
	vector<D> dest;
	vector<D> dir;
};

template <uint64_t D>
class camera {
typedef vector<D-1> vfov;
typedef vector<D-1, uint64_t> vres;
typedef vector<D> vec;
public:
	camera(vec pos, vfov fov, vres res)
		: pos(pos), fov(fov), res(res) {
		
	}

	array<ray<D>> Rays() {
		auto pxs = Pixels();
		auto r = array<ray<D>>(pxs.Len());
		for (uint64_t i=0; i<r.Len(); i++) {
			vector<D> dir = pos;
			r[i] = ray<D>(pos, dir);
		}
		return r;
	}

//protected:
	array<vector<D-1>> Pixels() {
		uint64_t pc = 1;
		for (uint64_t i=0; i<res.Len(); i++)
			pc *= res[i];
		auto rs = array<vector<D-1>>(pc);
	
		uint64_t i = 0;
		vector<D-1> v;
		while (i < pc) {
			rs[i] = v;
			for (uint64_t j=0; j<v.Len(); j++) {
				v[j]++;
				if (v[j] == res[j])
					v[j] = 0;
				else break;
			}
			i++;
		}
		return rs;
	}

private:
	vec pos;
	vfov fov;
	vres res;
};

int main() {
	camera<DIMS> cam (
		vector<DIMS>(0),
		vector<DIMS-1>(50),
		vector<DIMS-1, uint64_t>(5)
	);

	cam.Pixels().foreach(
		[](vector<DIMS-1> p) {
			std::cout << p << std::endl;
		}
	);

	cam.Rays().foreach(
		[](ray<DIMS> r) {
//			std::cout << r.dir << " " << r.dest << std::endl;
		}
	);
	return 0;
}
