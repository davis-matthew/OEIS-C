/*
 * A287870  The extended Wythoff array (the Wythoff array with two extra
 *          columns) read by antidiagonals downwards.
 * https://oeis.org/A287870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the extended Wythoff array – each
 *            row (indexed by i ≥ 0) consists of the row index, the lower
 *            Wythoff number floor((i+1)·φ), and then continues by the
 *            Fibonacci recurrence – along antidiagonals downwards. The first
 *            few values are: a(1)=0, a(2)=1, a(3)=1, a(4)=1, a(5)=3, …
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Find the row i and column j (0‑based) for the n‑th element in the
   antidiagonal traversal of the extended Wythoff array.
   ----------------------------------------------------------- */
static inline void get_ij(idx_t n, idx_t *i, idx_t *j)
{
    idx_t t = n - 1;          /* 0‑based offset */
    idx_t lo = 0, hi = 0;

    /* find hi such that cum(hi) > t,
       where cum(s) = (s+1)(s+2)/2  (total terms up to diagonal s) */
    while (true) {
        __int128 cum = (__int128)(hi + 1) * (hi + 2) / 2;
        if (cum > t)
            break;
        hi = hi * 2 + 1;           /* grow search range */
    }

    /* binary search for minimal s with cum(s) > t */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 cum_mid = (__int128)(mid + 1) * (mid + 2) / 2;
        if (cum_mid > t)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = hi;                                 /* diagonal index */

    __int128 prev_cnt = (__int128)s * (s + 1) / 2; /* terms before this diagonal */
    idx_t offset = t - (idx_t)prev_cnt;            /* position inside diagonal   */
    *i = offset;
    *j = s - offset;
}

/* ------------------------------------------------------------------
   Integer square root of a 128‑bit value, returning the floor.
   ----------------------------------------------------------- */
static inline seq_t isqrt_uint128(__int128 x)
{
    seq_t lo = 0, hi = 1;
    /* grow hi until hi*hi > x */
    while ((__int128)hi * hi <= x)
        hi <<= 1;

    while (hi - lo > 1) {
        seq_t mid = lo + ((hi - lo) >> 1);
        if ((__int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------
   Floor of n·φ where φ = (1+√5)/2, computed with integer arithmetic.
   ----------------------------------------------------------- */
static inline seq_t floor_phi(idx_t n)
{
    /* compute floor( n·√5 ) = floor( √(5·n²) ) */
    __int128 xn = (__int128)n * n;
    __int128 prod = (__int128)5 * xn;
    seq_t sqrt5 = isqrt_uint128(prod);
    /* floor( (n + n·√5) / 2 ) = floor( (n + sqrt5) / 2 )  */
    return ((seq_t)n + sqrt5) / 2;
}

/* ------------------------------------------------------------------
   Main sequence function.
   ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t i, j;
    get_ij(n, &i, &j);

    /* Column 0: the row index itself */
    seq_t val0 = (seq_t)i;
    if (j == 0)
        return val0;

    /* Column 1: floor((i+1)·φ) */
    seq_t val1 = floor_phi(i + 1ULL);
    if (j == 1)
        return val1;

    /* Columns >=2: Fibonacci recurrence */
    seq_t prevprev = val0;
    seq_t prev = val1;
    seq_t cur = 0;
    for (idx_t k = 2; k <= j; ++k) {
        cur = prev + prevprev;
        prevprev = prev;
        prev = cur;
    }
    return cur;
}
