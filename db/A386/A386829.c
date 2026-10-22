/*
 * A386829  a(n) = Sum_{k=0..n} 3^k * 2^(n-k) * binomial(2*n+1,k) *
 *          binomial(2*n-k,n-k).
 * https://oeis.org/A386829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * 2^(n-k) * binomial(2*n+1, k) *
 *            binomial(2*n-k, n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the supplied utilities for binomial coefficients
 * and powers of 2.  All intermediate values stay within the range of
 * seq_t; division is exact due to the combinatorial identities. */
 
#include <stdint.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow2.h"       /* seq_t pow2(idx_t exp); */

seq_t a(idx_t n)
{
    /* Pre‑compute some powers to avoid recomputing them in each loop. */
    seq_t pow3    = 1;                /* 3^k, starts at k = 0 */
    seq_t pow2_n  = pow2(n);          /* 2^(n-k), starts at k = 0 */

    seq_t binom1  = 1;                /* C(2n+1, 0) */
    seq_t binom2  = binomial(2 * n, n);   /* C(2n, n)  */

    seq_t result = 0;

    for (idx_t k = 0;; ++k) {
        /* term = 3^k * 2^(n-k) * binom1 * binom2 */
        result += pow3 * pow2_n * binom1 * binom2;

        if (k == n)
            break;                   /* finished all terms */

        /* Advance to k+1 */
        pow3 *= 3;                   /* 3^(k+1)               */
        pow2_n /= 2;                 /* 2^(n-(k+1))           */

        /* Update binom1: C(2n+1, k+1) = C(2n+1, k) * (2n+1-k)/(k+1) */
        seq_t factor1 = (seq_t)(2 * n + 1 - k);
        seq_t denom1  = (seq_t)(k + 1);
        binom1 = binom1 * factor1 / denom1;

        /* Update binom2: C(2n-k-1, n-k-1) = C(2n-k, n-k) * (n-k)/(2n-k) */
        seq_t numerator   = (seq_t)(n - k);          /* (n-k)   */
        seq_t denominator = (seq_t)(2 * n - k);      /* (2n-k)   */
        binom2 = binom2 * numerator / denominator;
    }

    return result;
}
