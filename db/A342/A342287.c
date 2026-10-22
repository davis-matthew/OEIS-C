/*
 * A342287  a(n) = number of n-variable nondegenerate self-dual truth-tables.
 * https://oeis.org/A342287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n-1} (-1)^k * C(n,k) * 2^(2^(n-1-k)) with a(0) =
 *            0
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Author: ChatGPT */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Pre‑compute the powers 2^(2^q) for q = 0 … n-1.
       pow[q] = 2^(2^q) */
    seq_t *pow = (seq_t*)malloc(n * sizeof(seq_t));
    if (!pow)
        return (seq_t)0;  /* out of memory – any value is fine */

    pow[0] = (seq_t)2;           /* q = 0 -> 2^(2^0) = 2^1 = 2 */
    for (idx_t q = 1; q < n; ++q)
        pow[q] = pow[q - 1] * pow[q - 1];   /* square each time */

    seq_t result = (seq_t)0;
    seq_t binom  = (seq_t)1;     /* C(n,0) = 1 */

    for (idx_t k = 0; k < n; ++k) {
        /* exponent q = n-1-k */
        idx_t q = n - 1 - k;
        seq_t term = binom * pow[q];
        if ((k & 1) == 0)
            result += term;
        else
            result -= term;

        /* update binomial coefficient to C(n,k+1) for next iteration */
        if (k + 1 < n) {
            binom = binom * (n - k);
            binom = binom / (k + 1);
        }
    }

    free(pow);
    return result;
}
