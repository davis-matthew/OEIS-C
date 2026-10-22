/*
 * A323589  a(n) = Product_{k=1..n-1} (k^k + (n-k)^(n-k)).
 * https://oeis.org/A323589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1}^{n-1} (k^k + (n-k)^(n-k))
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* offset: 0 (so a(0) = 1, a(1) = 1) */

/* This function performs an exact multiplication for all terms.
   It uses the helper big_pow to compute x^x for arbitrary-precision
   types. The routine works for any seq_t that supports the basic
   integer operations listed in the specification. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;               /* product over empty set */

    seq_t result = (seq_t)1;

    for (idx_t k = 1; k < n; ++k)
    {
        /* The base values fit in seq_t because n and k are indices. */
        seq_t base_k   = (seq_t)k;
        seq_t base_nk  = (seq_t)(n - k);

        seq_t pow_k  = big_pow(base_k, base_k);       /* k^k     */
        seq_t pow_nk = big_pow(base_nk, base_nk);     /* (n-k)^(n-k) */

        result *= (pow_k + pow_nk);
    }

    return result;
}
