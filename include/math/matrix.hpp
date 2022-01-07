#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <vector>
#include <cassert>

#include "math/vector3.hpp"
#include "math/vector4.hpp"

namespace engine {
    /**
     * Data is stored in COLUMN MAJOR layout.
     *
     * @tparam T The type of the elements in the matrix.
     * @tparam W The column count of the matrix.
     * @tparam H The row count of the matrix.
     */
    template<typename T, int W, int H>
    class MANA_EXPORT Matrix {
    public:
        static const size_t ROW_SIZE = W * sizeof(T);
        static const size_t COLUMN_SIZE = H * sizeof(T);

        //Public to ensure address of instance = first element of data, Array to ensure contiguous memory.
        T data[W * H];

        Matrix() : data() {};

        int width() const {
            return W;
        }

        int height() const {
            return H;
        }

        void set(int col, int row, T v) {
            assert(col >= 0 && col < W);
            assert(row >= 0 && row < H);
            data[H * col + row] = v;
        }

        const T &get(int col, int row) const {
            assert(col >= 0 && col < W);
            assert(row >= 0 && row < H);
            return data[H * col + row];
        }

        T &get(int col, int row) {
            assert(col >= 0 && col < W);
            assert(row >= 0 && row < H);
            return data[H * col + row];
        }

        Matrix<T, W, H> &operator+=(const Matrix<T, W, H> &other) {
            for (int col = 0; col < W; col++) {
                for (int row = 0; row < H; row++) {
                    data[H * col + row] += other.get(col, row);
                }
            }
            return *this;
        }

        Matrix<T, W, H> &operator-=(const Matrix<T, W, H> &other) {
            for (int col = 0; col < W; col++) {
                for (int row = 0; row < H; row++) {
                    data[H * col + row] -= other.get(col, row);
                }
            }
            return *this;
        }

        friend Matrix<T, W, H> operator+(const Matrix<T, W, H> &lhs, const Matrix<T, W, H> &rhs) {
            Matrix<T, W, H> ret;
            for (int x = 0; x < ret.width(); x++) {
                for (int y = 0; y < ret.height(); y++) {
                    ret.set(x, y, lhs.get(x, y) + rhs.get(x, y));
                }
            }
            return ret;
        }

        friend Matrix<T, W, H> operator-(const Matrix<T, W, H> &lhs, const Matrix<T, W, H> &rhs) {
            Matrix<T, W, H> ret;
            for (int x = 0; x < ret.width(); x++) {
                for (int y = 0; y < ret.height(); y++) {
                    ret.set(x, y, lhs.get(x, y) - rhs.get(x, y));
                }
            }
            return ret;
        }
    };

    // Multiply matrix by column vector
    MANA_EXPORT Vector4<float> operator*(const Matrix<float, 4, 4> &lhs, const Vector4<float> &rhs);

    MANA_EXPORT  Vector4<double> operator*(const Matrix<double, 4, 4> &lhs, const Vector4<double> &rhs);

    // Multiply matrix by column vector, convenience operators which call the vector 4 version with the w component set to 0
    MANA_EXPORT Vector3<float> operator*(const Matrix<float, 4, 4> &lhs, const Vector3<float> &rhs);

    MANA_EXPORT Vector3<double> operator*(const Matrix<double, 4, 4> &lhs, const Vector3<double> &rhs);

    // Multiply matrix by another matrix
    MANA_EXPORT Matrix<float, 4, 4> operator*(const Matrix<float, 4, 4> &lhs, const Matrix<float, 4, 4> &rhs);

    typedef Matrix<float, 2, 2> Mat2x2f;
    typedef Matrix<float, 2, 2> Mat2f;
    typedef Matrix<float, 2, 3> Mat2x3f;
    typedef Matrix<float, 2, 4> Mat2x4f;
    typedef Matrix<float, 3, 2> Mat3x2f;
    typedef Matrix<float, 3, 3> Mat3x3f;
    typedef Matrix<float, 3, 3> Mat3f;
    typedef Matrix<float, 3, 4> Mat3x4f;
    typedef Matrix<float, 4, 2> Mat4x2f;
    typedef Matrix<float, 4, 3> Mat4x3f;
    typedef Matrix<float, 4, 4> Mat4x4f;
    typedef Matrix<float, 4, 4> Mat4f;

    typedef Matrix<double, 2, 2> Mat2x2d;
    typedef Matrix<double, 2, 2> Mat2d;
    typedef Matrix<double, 2, 3> Mat2x3d;
    typedef Matrix<double, 2, 4> Mat2x4d;
    typedef Matrix<double, 3, 2> Mat3x2d;
    typedef Matrix<double, 3, 3> Mat3x3d;
    typedef Matrix<double, 3, 3> Mat3d;
    typedef Matrix<double, 3, 4> Mat3x4d;
    typedef Matrix<double, 4, 2> Mat4x2d;
    typedef Matrix<double, 4, 3> Mat4x3d;
    typedef Matrix<double, 4, 4> Mat4x4d;
    typedef Matrix<double, 4, 4> Mat4d;

    typedef Matrix<int, 2, 2> Mat2x2i;
    typedef Matrix<int, 2, 2> Mat2i;
    typedef Matrix<int, 2, 3> Mat2x3i;
    typedef Matrix<int, 2, 4> Mat2x4i;
    typedef Matrix<int, 3, 2> Mat3x2i;
    typedef Matrix<int, 3, 3> Mat3x3i;
    typedef Matrix<int, 3, 3> Mat3i;
    typedef Matrix<int, 3, 4> Mat3x4i;
    typedef Matrix<int, 4, 2> Mat4x2i;
    typedef Matrix<int, 4, 3> Mat4x3i;
    typedef Matrix<int, 4, 4> Mat4x4i;
    typedef Matrix<int, 4, 4> Mat4i;
}

#endif // MATRIX_HPP