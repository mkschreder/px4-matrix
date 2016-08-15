/**
 * @file Matrix.hpp
 *
 * A simple matrix template library.
 *
 * @author James Goppert <james.goppert@gmail.com>
 */

#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#if defined(SUPPORT_STDIOSTREAM)
#include <iostream>
#include <iomanip>
#endif // defined(SUPPORT_STDIOSTREAM)

#include "math.hpp"

namespace matrix
{

template <typename Type, size_t M>
class Vector;

template <typename Type, size_t M>
class SquareMatrix;

template<typename Type, size_t  M, size_t N>
class Matrix
{

public:

    Type _data[M][N];

    virtual ~Matrix() {};

    Matrix() :
        _data()
    {
        memset(_data, 0, sizeof(_data));
    }

    Matrix(const Type *data_) :
        _data()
    {
        memcpy(_data, data_, sizeof(_data));
    }

    Matrix(const Type data_[M][N]) :
        _data()
    {
        memcpy(_data, data_, sizeof(_data));
    }


    Matrix(const Matrix &other) :
        _data()
    {
        memcpy(_data, other._data, sizeof(_data));
    }

    /**
     * Accessors/ Assignment etc.
     */

    Type *data()
    {
        return _data[0];
    }

    inline Type operator()(size_t i, size_t j) const
    {
        return _data[i][j];
    }

    inline Type &operator()(size_t i, size_t j)
    {
        return _data[i][j];
    }

    Matrix<Type, M, N> & operator=(const Matrix<Type, M, N> &other)
    {
        if (this != &other) {
            memcpy(_data, other._data, sizeof(_data));
        }
        return (*this);
    }

    static const Matrix<Type, M, N> &Identity() {
        static Matrix<Type, M, N> res;
        res.setIdentity();
        return res;
    }

    /**
     * Matrix Operations
     */

    // this might use a lot of programming memory
    // since it instantiates a class for every
    // required mult pair, but it provides
    // compile time size_t checking
    template<size_t P>
    Matrix<Type, M, P> operator*(const Matrix<Type, N, P> &other) const
    {
        const Matrix<Type, M, N> &self = *this;
        Matrix<Type, M, P> res;
        res.setZero();

        for (size_t i = 0; i < M; i++) {
            for (size_t k = 0; k < P; k++) {
                for (size_t j = 0; j < N; j++) {
                    res(i, k) += self(i, j) * other(j, k);
                }
            }
        }

        return res;
    }

    Matrix<Type, M, N> emult(const Matrix<Type, M, N> &other) const
    {
        Matrix<Type, M, N> res;
        const Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                res(i , j) = self(i, j)*other(i, j);
            }
        }

        return res;
    }

    Matrix<Type, M, N> operator+(const Matrix<Type, M, N> &other) const
    {
        Matrix<Type, M, N> res;
        const Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                res(i , j) = self(i, j) + other(i, j);
            }
        }

        return res;
    }

    bool operator==(const Matrix<Type, M, N> &other) const
    {
        const Matrix<Type, M, N> &self = *this;
        static const Type eps = Type(1e-4);

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                if (fabs(self(i , j) - other(i, j)) > eps) {
                    return false;
                }
            }
        }

        return true;
    }

    Matrix<Type, M, N> operator-(const Matrix<Type, M, N> &other) const
    {
        Matrix<Type, M, N> res;
        const Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                res(i , j) = self(i, j) - other(i, j);
            }
        }

        return res;
    }

    // unary minus
    Matrix<Type, M, N> operator-() const
    {
        Matrix<Type, M, N> res;
        const Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                res(i , j) = -self(i, j);
            }
        }

        return res;
    }

    void operator+=(const Matrix<Type, M, N> &other)
    {
        Matrix<Type, M, N> &self = *this;
        self = self + other;
    }

    void operator-=(const Matrix<Type, M, N> &other)
    {
        Matrix<Type, M, N> &self = *this;
        self = self - other;
    }

    template<size_t P>
    void operator*=(const Matrix<Type, N, P> &other)
    {
        Matrix<Type, M, N> &self = *this;
        self = self * other;
    }

    /**
     * Scalar Operations
     */

    Matrix<Type, M, N> operator*(Type scalar) const
    {
        Matrix<Type, M, N> res;
        const Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                res(i , j) = self(i, j) * scalar;
            }
        }

        return res;
    }

    inline Matrix<Type, M, N> operator/(Type scalar) const
    {
        return (*this)*(1/scalar);
    }

    Matrix<Type, M, N> operator+(Type scalar) const
    {
        Matrix<Type, M, N> res;
        const Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                res(i , j) = self(i, j) + scalar;
            }
        }

        return res;
    }

    inline Matrix<Type, M, N> operator-(Type scalar) const
    {
        return (*this) + (-1*scalar);
    }

    void operator*=(Type scalar)
    {
        Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                self(i, j) = self(i, j) * scalar;
            }
        }
    }

    void operator/=(Type scalar)
    {
        Matrix<Type, M, N> &self = *this;
        self = self * (1.0f / scalar);
    }

    inline void operator+=(Type scalar)
    {
        *this = (*this) + scalar;
    }

    inline void operator-=(Type scalar)
    {
        *this = (*this) - scalar;
    }


    /**
     * Misc. Functions
     */

    void write_string(char * buf, size_t n) const
    {
        buf[0] = '\0'; // make an empty string to begin with (we need the '\0' for strlen to work)
        const Matrix<Type, M, N> &self = *this;
        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                snprintf(buf + strlen(buf), n - strlen(buf), "\t%.2g", double(self(i, j))); // directly append to the string buffer
            }
            snprintf(buf + strlen(buf), n - strlen(buf), "\n");
        }
    }

    void print() const
    {
        char buf[200];
        write_string(buf, 200);
        printf("%s\n", buf);
    }

    // alias
    inline Matrix<Type, N, M> transposed() const {
        return transpose();
    }
    Matrix<Type, N, M> transpose() const
    {
        Matrix<Type, N, M> res;
        const Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                res(j, i) = self(i, j);
            }
        }

        return res;
    }

    // tranpose alias
    inline Matrix<Type, N, M> T() const
    {
        return transpose();
    }

    template<size_t P, size_t Q>
    Matrix<Type, P, Q> slice(size_t x0, size_t y0) const
    {
        Matrix<Type, P, Q> res(&(_data[x0][y0]));
        return res;
    }

    template<size_t P, size_t Q>
    void set(const Matrix<Type, P, Q> &m, size_t x0, size_t y0)
    {
        Matrix<Type, M, N> &self = *this;
        for (size_t i = 0; i < P; i++) {
            for (size_t j = 0; j < Q; j++) {
                self(i + x0, j + y0) = m(i, j);
            }
        }
    }

    Matrix<Type, 1, N> getRow(size_t i) {
        Matrix<Type, M, N> &self = *this;
        Matrix<Type, 1, N> res;
        for(size_t c = 0; c < N; c++) {
            res(0, c) = self(i, c);
        }
        return res;
    }

    Matrix<Type, M, 1> getCol(size_t j) {
        Matrix<Type, M, N> &self = *this;
        Matrix<Type, M, 1> res;
        for(size_t c = 0; c < M; c++) {
            res(c, 0) = self(c, j);
        }
        return res;
    }

    void setRow(size_t i, const Matrix<Type, 1, N> &row)
    {
        Matrix<Type, M, N> &self = *this;
        for (size_t j = 0; j < N; j++) {
            self(i, j) = row(0, j);
        }
    }

    void setCol(size_t j, const Matrix<Type, M, 1> &col)
    {
        Matrix<Type, M, N> &self = *this;
        for (size_t i = 0; i < M; i++) {
            self(i, j) = col(i, 0);
        }
    }

    void setZero()
    {
        memset(_data, 0, sizeof(_data));
    }

    void setAll(Type val)
    {
        Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            for (size_t j = 0; j < N; j++) {
                self(i, j) = val;
            }
        }
    }

    inline void setOne()
    {
        setAll(1);
    }

    void setIdentity()
    {
        setZero();
        Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M && i < N; i++) {
            self(i, i) = 1;
        }
    }

    inline void swapRows(size_t a, size_t b)
    {
        if (a == b) {
            return;
        }

        Matrix<Type, M, N> &self = *this;

        for (size_t j = 0; j < N; j++) {
            Type tmp = self(a, j);
            self(a, j) = self(b, j);
            self(b, j) = tmp;
        }
    }

    inline void swapCols(size_t a, size_t b)
    {
        if (a == b) {
            return;
        }

        Matrix<Type, M, N> &self = *this;

        for (size_t i = 0; i < M; i++) {
            Type tmp = self(i, a);
            self(i, a) = self(i, b);
            self(i, b) = tmp;
        }
    }

    Matrix<Type, M, N> abs()
    {
        Matrix<Type, M, N> r;
        for (size_t i=0; i<M; i++) {
            for (size_t j=0; j<N; j++) {
                r(i,j) = Type(fabs((*this)(i,j)));
            }
        }
        return r;
    }

    Type max()
    {
        Type max_val = (*this)(0,0);
        for (size_t i=0; i<M; i++) {
            for (size_t j=0; j<N; j++) {
                Type val = (*this)(i,j);
                if (val > max_val) {
                    max_val = val;
                }
            }
        }
        return max_val;
    }

    Type min()
    {
        Type min_val = (*this)(0,0);
        for (size_t i=0; i<M; i++) {
            for (size_t j=0; j<N; j++) {
                Type val = (*this)(i,j);
                if (val < min_val) {
                    min_val = val;
                }
            }
        }
        return min_val;
    }

    Matrix<Type, M, N> limited(const Matrix<Type, M, N> &_min, const Matrix<Type, M, N> &_max) {
        Matrix<Type, M, N> res;
        for (size_t i=0; i<M; i++) {
            for (size_t j=0; j<N; j++) {
                float r = (*this)(i, j);
                if(r < _min(i, j)) r = _min(i, j);
                if(r > _max(i, j)) r = _max(i, j);
                res(i, j) = r;
            }
        }
        return res;
    }
};

template<typename Type, size_t M, size_t N>
Matrix<Type, M, N> zeros() {
    Matrix<Type, M, N> m;
    m.setZero();
    return m;
}

template<typename Type, size_t M, size_t N>
Matrix<Type, M, N> ones() {
    Matrix<Type, M, N> m;
    m.setOne();
    return m;
}

template<typename Type, size_t  M, size_t N>
Matrix<Type, M, N> operator*(Type scalar, const Matrix<Type, M, N> &other)
{
    return other * scalar;
}

template<typename Type, size_t  M, size_t N>
bool isEqual(const Matrix<Type, M, N> &x,
             const Matrix<Type, M, N> & y) {
    if (!(x == y)) {
        char buf_x[100];
        char buf_y[100];
        x.write_string(buf_x, 100);
        y.write_string(buf_y, 100);
        printf("not equal\nx:\n%s\ny:\n%s\n", buf_x, buf_y);
    }
    return x == y;
}

template<typename Type, size_t M>
inline Matrix<Type, M, M> inversed(Matrix<Type, M, M> const &m) {
    return SquareMatrix<Type, M>(m).I();
}

#if defined(SUPPORT_STDIOSTREAM)
template<typename Type, size_t  M, size_t N>
std::ostream& operator<<(std::ostream& os,
                         const matrix::Matrix<Type, M, N>& matrix)
{
    for (size_t i = 0; i < M; ++i) {
        os << "[";
        for (size_t j = 0; j < N; ++j) {
            os << std::setw(10) << static_cast<double>(matrix(i, j));
            os << "\t";
        }
        os << "]" << std::endl;
    }
    return os;
}
#endif // defined(SUPPORT_STDIOSTREAM)

} // namespace matrix

/* vim: set et fenc=utf-8 ff=unix sts=0 sw=4 ts=4 : */
