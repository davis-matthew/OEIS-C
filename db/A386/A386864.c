/*
 * A386864  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(4*n+1,k) *
 *          binomial(4*n-k-1,n-k).
 * https://oeis.org/A386864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(4*n+1,k) *
 *            binomial(4*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* a(0) = 1 */
        return (seq_t)1;

    idx_t four_n = 4 * n;
    seq_t sum = 0;

    seq_t p = pow2(n);                 /* current power of 2: 2^(n-k) */
    seq_t C = (seq_t)1;                /* binomial(4*n+1,0) */
    seq_t D = binomial(four_n - 1, n); /* binomial(4*n-1,n)  */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t t = p * C;               /* 2^(n-k) * binomial(4*n+1,k) */
        t = t * D;                     /* * binomial(4*n-k-1,n-k)     */
        sum += t;

        if (k == n)                   /* finished all terms */
            break;

        /* Prepare for next iteration (k+1) */
        p = p / (seq_t)2;             /* 2^(n-(k+1))                */
        C = C * (seq_t)(four_n + 1 - k) / (seq_t)(k + 1);
        D = D * (seq_t)(n - k) / (seq_t)(four_n - k - 1);
    }

    return sum;
}
