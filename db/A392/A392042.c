/*
 * A392042  a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-3*k) *
 *          binomial(k,3*(n-3*k)).
 * https://oeis.org/A392042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} 2^k * 3^(n-3*k) *
 *            binomial(k,3*(n-3*k))
 * Range:     u8 n <= 15, u16 n <= 27, u32 n <= 47, u64 n <= 94, u128 n <= 183,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* The types seq_t and idx_t are provided by the build environment. */
#include "../utils/binomial.h"
/* pow2 and pow3 utilities are also available. */
#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Compute one term of the sequence. */
seq_t a(idx_t n)
{
    seq_t result = 0;
    /* Iterate over all k from 0 to floor(n/3) */
    for (idx_t k = 0; k <= n / 3; ++k)
    {
        /* Number of 3's needed to make the exponent zero: m = 3*(n-3k) */
        idx_t m = 3 * (n - 3 * k);
        /* Skip if m > k since binomial(k,m) would be zero */
        if (m > k)
            continue;

        /* Compute the term: 2^k * 3^(n-3k) * binomial(k,m) */
        seq_t term = pow2(k);
        term *= pow3(n - 3 * k);
        term *= binomial(k, m);

        result += term;
    }
    return result;
}
