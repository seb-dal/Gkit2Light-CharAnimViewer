#pragma once

#include <vector>
#include <cassert>

template <class T>
class Array2D {
public:
	Array2D(const int sizeX, const int sizeY) :size_X(sizeX), size_Y(sizeY) {
		assert(sizeX > 0 && size_Y > 0);
		listElem.resize(size_X * size_Y);
	}
	const int sizeX() const { return size_X; }
	const int sizeY() const { return size_Y; }
	const int sizeTotal() const { return size_X * size_Y; }


	T& operator()(const size_t x) {
		assert(size_X != 0 && size_Y != 0); // not initialized
		assert(x < size_X* size_Y); // out of range 
		assert(x >= 0); // negatif size

		return listElem[x];
	}

	T& operator[](const size_t x) {
		assert(size_X != 0 && size_Y != 0); // not initialized
		assert(x < size_X* size_Y); // out of range 
		assert(x >= 0); // negatif size

		return listElem[x];
	}

	T operator()(const size_t x) const {
		assert(size_X != 0 && size_Y != 0); // not initialized
		assert(x < size_X* size_Y); // out of range 
		assert(x >= 0); // negatif size

		return listElem[x];
	}

	T& operator()(const size_t x, const size_t y) {
		assert(size_X != 0 && size_Y != 0); // not initialized
		assert(x < size_X&& y < size_Y); // out of range 
		assert(x >= 0 && y >= 0); // negatif size

		return listElem[y * size_X + x];
	}

	T operator()(const size_t x, const size_t y) const {
		assert(size_X != 0 && size_Y != 0); // not initialized
		assert(x < size_X&& y < size_Y); // out of range 
		assert(x >= 0 && y >= 0); // negatif size

		return listElem[y * size_X + x];
	}

	std::vector<T>& get() { return listElem; }

protected:
	int size_X, size_Y;
	std::vector<T> listElem;
};

