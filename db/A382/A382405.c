/*
 * A382405  a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(n+k,k) * 2^(n-k).
 * https://oeis.org/A382405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^2 * binomial(n+k,k) * 2^(n-k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    seq_t b1  = 1;                 /* binomial(n,0) */
    seq_t b2  = 1;                 /* binomial(n+0,0) = 1   */
    seq_t pow = pow2(n);           /* 2^(n-0) = 2^n        */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = b1 * b1;      /* binomial(n,k)^2      */
        term = term * b2;          /* * binomial(n+k,k)    */
        term = term * pow;         /* * 2^(n-k)            */
        sum += term;

        if (k == n) break;        /* last iteration, no update */

        seq_t k1 = (seq_t)(k + 1);

        /* update binomial(n,k+1) */
        b1 = (b1 * (n - k)) / k1;

        /* update binomial(n+(k+1),k+1) = binomial(n+k+1,k+1) */
        b2 = (b2 * (n + k + 1)) / k1;

        /* update power of two */
        pow >>= 1;
    }
    return sum;
}
