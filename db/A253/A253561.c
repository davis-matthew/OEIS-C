/*
 * A253561  Square array read by antidiagonals: A(row,col) =
 *          A122111(A246278(row,col)).
 * https://oeis.org/A253561
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(r,c) = t*L^(r-1), t = A122111(2c), L = largest prime factor of
 *            t, read by antidiagonals (row r = j+1, col c = d-j)
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 150, u64 n <= 458,
 *            u128 n <= 466, big any n
 * Validated: standard, pass, 94 terms through n = 450 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a253561_isp(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t f = 2; f * f <= x; ++f)
        if (x % f == 0) return false;
    return true;
}

static uint64_t a253561_nthp(unsigned k)
{
    uint64_t x = 1;
    unsigned c = 0;
    while (c < k) {
        ++x;
        if (a253561_isp(x)) ++c;
    }
    return x;
}

static unsigned a253561_pi(uint64_t p)
{
    unsigned c = 0;
    for (uint64_t x = 2; x <= p; ++x)
        if (a253561_isp(x)) ++c;
    return c;
}

/* Computes t = A122111(m) and the largest prime of t via conjugate partition */
static seq_t a253561_conj(uint64_t m, seq_t *lpf)
{
    unsigned idx[128];
    unsigned cnt = 0;
    unsigned mx = 0;
    uint64_t f = 2;
    while (f * f <= m) {
        if (m % f == 0) {
            unsigned i = a253561_pi(f);
            while (m % f == 0) {
                m /= f;
                idx[cnt++] = i;
                if (i > mx) mx = i;
            }
        }
        ++f;
    }
    if (m > 1) {
        unsigned i = a253561_pi(m);
        idx[cnt++] = i;
        if (i > mx) mx = i;
    }
    seq_t res = 1;
    seq_t last = 1;
    for (unsigned j = 1; j <= mx; ++j) {
        unsigned c = 0;
        for (unsigned i = 0; i < cnt; ++i)
            if (idx[i] >= j) ++c;
        uint64_t p = a253561_nthp(c);
        res *= (seq_t)p;
        if (j == 1) last = (seq_t)p;
    }
    *lpf = last;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n - 2;
    uint64_t d = 1;
    while (d * (d + 1) / 2 <= k) ++d;
    uint64_t j = k - d * (d - 1) / 2;
    uint64_t row = j + 1;
    uint64_t col = d - j;

    seq_t L = 1;
    seq_t t = a253561_conj(2 * col, &L);
    seq_t r = t;
    for (uint64_t i = 1; i < row; ++i) r *= L;
    return r;
}
