/*
 * A244116  Triangle read by rows: coefficients T(n,k) of a binomial
 *          decomposition of 1 as Sum_{k=0..n} T(n,k)*binomial(n,k).
 * https://oeis.org/A244116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 19, i16 n <= 34, i32 n <= 75, i64 n <= 169,
 *            i128 n <= 433, big any n
 * Helpers:   _floor_isqrt_u64, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A244116: a(n) is the n-th entry of the triangle T(n,k) defined by
   T(n,k) = (1-k)^(k-1) * k^(n-k) for k>0 and T(n,0)=0^n (conventionally,
   T(0,0)=1).  The triangle is read row‑by‑row starting at row 0. */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t t = _floor_isqrt_u64(8*n + 1);
    idx_t r = (t - 1) / 2;

    /* Compute the offset within the row */
    __uint128_t start128 = (__uint128_t)r * (r + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t k = n - start;                /* 0 <= k <= r */

    /* Handle k = 0 separately */
    if (k == 0) {
        return r == 0 ? 1 : 0;
    }

    /* Compute (1-k)^(k-1) * k^(r-k) */
    seq_t base1 = (seq_t)1 - (seq_t)k;
    seq_t part1 = big_pow(base1, (seq_t)(k - 1));

    seq_t base2 = (seq_t)k;
    seq_t part2 = big_pow(base2, (seq_t)(r - k));

    return part1 * part2;
}
