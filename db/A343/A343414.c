/*
 * A343414  a(n) = Sum_{i=1..n+1} prime(i)*Sum_{j=0..i-1} binomial(n,j).
 * https://oeis.org/A343414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n+1} prime(i) * Sum_{j=0..i-1} binomial(n,j)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 51, u128 n <= 113,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Computes OEIS sequence A343414 */
seq_t a(idx_t n)
{
    seq_t total  = 0;
    seq_t c      = 1;       /* C(n,0) */
    seq_t prefix = 0;       /* Σ_{j=0..k} C(n,j) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* add current binomial coefficient to the running prefix sum */
        prefix += c;

        /* i = k+1, so the prime required is prime(k+1) */
        seq_t prime_k1 = (seq_t)get_prime((idx_t)(k + 1));

        /* accumulate the contribution for this i */
        total += prime_k1 * prefix;

        /* compute next binomial coefficient C(n,k+1) if needed */
        if (k < n)
        {
            seq_t num = c * (seq_t)(n - k);
            seq_t den = (seq_t)(k + 1);
            c = num / den;   /* exact division */
        }
    }
    return total;
}
