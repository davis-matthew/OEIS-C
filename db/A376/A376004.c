/*
 * A376004  Limiting matrix {m_n}, where m_0 = 1 and m_{i+1} = [[m_i, A(m_i)],
 *          [B(m_i), C(m_i)]], read by antidiagonals, and A adds the
 *          corresponding x-coords to every element, B subtracts it, and C
 *          adds the corresponding y-coords.
 * https://oeis.org/A376004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look, tabl
 * Formula:   a(n) = values read by antidiagonals from the limiting matrix m∞,
 *            where m0 = [1] and m_{i+1} = [[m_i, A(m_i)], [B(m_i), C(m_i)]] A
 *            adds the local x-coordinate to each element, B subtracts that
 *            local x, C adds the local y-coordinate
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recursion below evaluates the value at the coordinates (x , y)
   corresponding to index n, using only integer operations. */

#include <stdint.h>

static inline idx_t diagonal_of(idx_t n)
{
    /* Find the smallest k such that (k+1)(k+2)/2 >= n.
       Use binary search with __int128 to avoid overflow. */
    idx_t lo = 0, hi = 1;
    while (( (__int128)(hi + 1) * (hi + 2) / 2 ) < n)
        hi <<= 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (( (__int128)(mid + 1) * (mid + 2) / 2 ) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Find diagonal index k (sum s = x + y). */
    idx_t k = diagonal_of(n);
    idx_t prev = k * (k + 1) / 2;              /* elements before this diagonal */
    idx_t off  = n - prev - 1;                 /* 0‑based offset on the diagonal */
    idx_t x = k - off;                         /* coordinate mapping: (k-i, i) */
    idx_t y = off;

    seq_t val = 1;                             /* base value f(0,0) */

    /* Evaluate recursively using only integer arithmetic. */
    while (x || y) {
        /* largest power of two <= max(x, y) (max(x,y) > 0 here) */
        idx_t m = x > y ? x : y;
        idx_t t = 1ULL << (63 - __builtin_clzll(m));

        if (x >= t && y < t) {                /* top‑right block */
            val += (seq_t)(x - t);
            x -= t;
        } else if (x < t && y >= t) {          /* bottom‑left block */
            val -= (seq_t)x;
            y -= t;
        } else {                               /* bottom‑right block */
            val += (seq_t)(y - t);
            x -= t;
            y -= t;
        }
    }
    return val;
}
