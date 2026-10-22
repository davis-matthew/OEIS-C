/*
 * A354588  Number of marked chord diagrams (linear words in which each letter
 *          appears twice) with n chords, whose intersection graph is
 *          connected and distance-hereditary.
 * https://oeis.org/A354588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..n} C(n+k, n) * C(2*(n+1)+k, n-k) *
 *            2^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    idx_t m = n + 1;                 /* n+1 as idx_t */
    __uint128_t sum = 0;             /* 128‑bit accumulator for safety */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t t1 = binomial(n + k, n);                     /* C(n+k, n)   */
        seq_t t2 = binomial(2 * m + k, n - k);             /* C(2(n+1)+k, n-k) */
        seq_t pk = pow2(k);                                /* 2^k         */

        __uint128_t prod = ( __uint128_t )t1 * ( __uint128_t )t2;
        prod *= ( __uint128_t )pk;
        sum += prod;
    }

    sum /= m;                            /* divide by (n+1) */
    return (seq_t)sum;                    /* cast to result type */
}
