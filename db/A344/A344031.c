/*
 * A344031  Irregular triangle read by rows: T(n,k) = [x^k]p_n(x), where
 *          (p_n(x)/x^(3n)) * exp(-1/x^2) is the n-th derivative of
 *          exp(-1/x^2), n >= 1, 0 <= k <= 2*n-2.
 * https://oeis.org/A344031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   a(n) is the coefficient of x^k in the polynomial p_r(x) where r
 *            = ceil(sqrt(n)) and k = n - (r-1)^2 - 1. The polynomials satisfy
 *            p_0(x)=1 and for n≥0 p_{n+1}(x) = p_n'(x) - (3*n*x^2 -
 *            2)*p_n(x). The coefficients T(n,k) = [x^k]p_n(x) obey T(1,0) =
 *            2; T(n+1,k) = (k+1)*T(n,k+1) + 2*T(n,k) - 3*n*T(n,...
 * Range:     i8 n <= 11, i16 n <= 30, i32 n <= 75, i64 n <= 210,
 *            i128 n <= 604, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static inline idx_t floor_sqrt(idx_t v)
{
    idx_t lo = 0, hi = (idx_t)1 << 32; /* sqrt(2^64-1) < 2^32 */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (( (__uint128_t)mid * mid ) <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

static inline seq_t *allocate_array(idx_t len)
{
#ifdef __cplusplus
    return new seq_t[len]();          /* value‑initialize each element */
#else
    return (seq_t *)calloc((size_t)len, sizeof(seq_t));
#endif
}

static inline void free_array(seq_t *p)
{
#ifdef __cplusplus
    delete [] p;
#else
    free(p);
#endif
}

seq_t a(idx_t n)
{
    /* invalid indices produce 0 */
    if (n == 0) return 0;

    /* row and column indices */
    idx_t r = floor_sqrt(n - 1) + 1;          /* ceil(sqrt(n)) */
    idx_t prev_sq = (idx_t)((__uint128_t)(r - 1) * (r - 1));
    idx_t k = n - prev_sq - 1;                /* 0‑based column in row r */

    /* row 1 is the base */
    if (r == 1) {
        return (seq_t)2;
    }

    /* dynamic computation of rows */
    seq_t *prev = allocate_array(1);
    prev[0] = (seq_t)2;                       /* T(1,0) = 2 */

    for (idx_t i = 2; i <= r; ++i) {
        idx_t len_cur = 2 * i - 1;             /* length of row i (indices 0..2i-2) */
        seq_t *cur = allocate_array(len_cur);

        idx_t nprev = i - 1;                   /* previous row index */
        idx_t len_prev = 2 * nprev - 1;         /* length of previous row */

        for (idx_t kk = 0; kk < len_cur; ++kk) {
            seq_t val = 0;

            /* (k+1) * T(n,k+1) */
            if (kk + 1 < len_prev)
                val += (seq_t)(kk + 1) * prev[kk + 1];

            /* 2 * T(n,k) */
            if (kk < len_prev)
                val += 2 * prev[kk];

            /* –3*n * T(n,k-2) */
            if (kk >= 2)
                val -= (seq_t)(3 * nprev) * prev[kk - 2];

            cur[kk] = val;
        }

        free_array(prev);
        prev = cur;
    }

    seq_t res = prev[k];
    free_array(prev);
    return res;
}
