/*
 * A278736  Number of size-4 cliques in all simple labeled graphs on n nodes.
 * https://oeis.org/A278736
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,4) * 2^(binomial(n,2)-6)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 4. For indices below the offset,
       a(n) is undefined – returning 0 is harmless. */
    if (n < 4)
        return 0;

    /* Compute the two binomial coefficients needed. */
    seq_t c4 = binomial(n, 4);   /* n choose 4 */
    seq_t c2 = binomial(n, 2);   /* n choose 2 */

    /* Working exponent (fits in idx_t for all n that fit in the b‑file). */
    idx_t exp = (idx_t)c2 - 6;    /* (n choose 2) – 6 */

    /* Final value: c4 * 2^exp.  pow2 returns a seq_t. */
    return c4 * pow2(exp);
}
