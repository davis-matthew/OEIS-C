/*
 * A388410  a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(n+3,k).
 * https://oeis.org/A388410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n,k) * binomial(n+3,k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute 3^n once */
    seq_t pow3_power = pow3(n);      /* 3^n */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow3_power * binomial(n, k) * binomial(n + 3, k);
        sum += term;
        if (k < n) {
            pow3_power /= 3; /* advance to 3^(n-(k+1)) */
        }
    }

    return sum;
}
