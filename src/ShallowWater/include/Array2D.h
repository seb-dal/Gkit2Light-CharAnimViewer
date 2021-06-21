#include <cassert>
#include <vector>
#include <src/gKit/vec.h>

#define noAssert 1

class Array2D {
public:
	Array2D() { m_dimX = m_dimY = 0; }
	void init(const int DIMX, const int DIMY) { m_dimX = DIMX; m_dimY = DIMY; m_data.resize(m_dimX * m_dimY); }
	void setAll(const float v) { m_data.assign(m_dimX * m_dimY, v); }
	const int dimX() const { return m_dimX; }
	const int dimY() const { return m_dimY; }

	float& operator()(const int x, const int y) {
#if noAssert == 0
		assert(m_dimX != 0 && m_dimY != 0); // not initialized
		assert(x < m_dimX&& y < m_dimY); // out of range 
		assert(x >= 0 && y >= 0); // negatif size
#endif
		return m_data[y * m_dimX + x];
	}

	float operator()(const int x, const int y) const {
#if noAssert == 0
		assert(m_dimX != 0 && m_dimY != 0); // not initialized
		assert(x < m_dimX&& y < m_dimY); // out of range 
		assert(x >= 0 && y >= 0); // negatif size
#endif

		return m_data[y * m_dimX + x];
	}

	float interpolate(const float x, const float y) const {
		if ((x < 0) || (x > m_dimX) || (y < 0) || (y > m_dimY)) return 0.f;
		int X = int(x);
		int Y = int(y);
		int Xp = X + 1;
		int Yp = Y + 1;

		if ((Xp >= m_dimX) || (Yp >= m_dimY)) return operator()(X, Y);
		float IX_b = (x - X) * operator()(Xp, Y) + (Xp - x) * operator()(X, Y);
		float IX_h = (x - X) * operator()(Xp, Yp) + (Xp - x) * operator()(X, Yp);
		return (y - Y) * IX_h + (Yp - y) * IX_b;
	}
protected:
	std::vector<float> m_data; // OU float* m_data
	int m_dimX, m_dimY;
};



class Array2D_bool {
public:
	Array2D_bool() { m_dimX = m_dimY = 0; }
	void init(const int DIMX, const int DIMY) { m_dimX = DIMX; m_dimY = DIMY; m_data.resize(m_dimX * m_dimY); }
	void setAll(const bool v) { m_data.assign(m_dimX * m_dimY, v); }
	const int dimX() const { return m_dimX; }
	const int dimY() const { return m_dimY; }

	void operator()(const int x, const int y, const bool v) {
#if noAssert == 0
		assert(m_dimX != 0 && m_dimY != 0); // not initialized
		assert(x < m_dimX&& y < m_dimY); // out of range 
		assert(x >= 0 && y >= 0); // negatif size
#endif

		m_data[y * m_dimX + x] = v;
	}

	bool operator()(const int x, const int y) const {
#if noAssert == 0
		assert(m_dimX != 0 && m_dimY != 0); // not initialized
		assert(x < m_dimX&& y < m_dimY); // out of range 
		assert(x >= 0 && y >= 0); // negatif size
#endif

		return m_data[y * m_dimX + x];
	}
protected:
	std::vector<bool> m_data; // OU float* m_data
	int m_dimX, m_dimY;
};

/*int main() {
	//test
		Array2D a;
		a.init(4, 4);
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				a(i, j) = (i + j) * i + j;
			}
		}

		Array2D b = a;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				assert(a(i, j) == b(i, j));// a == b
				b(i, j) += i + j + 1;
				assert(a(i, j) != b(i, j));
			}
		}
}*/

