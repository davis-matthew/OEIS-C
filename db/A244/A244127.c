/*
 * A244127  Triangle read by rows: terms T(n,k) of a binomial decomposition of
 *          2^n-1 as Sum(k=0..n)T(n,k).
 * https://oeis.org/A244127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,c) where T(r,c) =
 *            (1+c)^(c-1)*(1-c)^(r-c)*binomial(r,c) for c>0, T(r,0)=0 and
 *            (r,c) is obtained from n by mapping the triangle read by rows: r
 *            = floor((sqrt(8*n+1)-1)/2), c = n - r*(r+1)/2
 * Range:     i8 n <= 17, i16 n <= 31, i32 n <= 60, i64 n <= 145,
 *            i128 n <= 393, big any n
 * Helpers:   binomial, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_isqrt_u64.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find row r and column c from flattened index n */
    idx_t d = 8ULL * n + 1;                      /* 8*n + 1 */
    idx_t s = floor_isqrt_u64(d);                /* floor(sqrt(d)) */
    idx_t r = (s - 1) / 2;                       /* row index */
    idx_t t = r * (r + 1) / 2;                   /* terms before row r */
    idx_t c = n - t;                              /* column index within row */

    if (c == 0) return 0;                       /* T(r,0) = 0 by definition */

    /* Compute (1 + c)^(c-1) */
    seq_t pow1 = 1;
    seq_t base1 = (seq_t)(1 + c);
    for (idx_t i = 1; i < c; ++i)
        pow1 *= base1;

    /* Compute (1 - c)^(r - c) */
    seq_t pow2 = 1;
    seq_t base2 = (seq_t)(1) - (seq_t)c;
    for (idx_t i = 0; i < r - c; ++i)
        pow2 *= base2;

    /* Binomial coefficient binomial(r,c) */
    seq_t bin = binomial(r, c);

    /* Final result */
    seq_t result = pow1 * pow2;
    result *= bin;
    return result;
}
