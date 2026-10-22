/*
 * A306390  Size of one subtree in the unlabeled binary rooted tree shape of
 *          size n whose leaf-labeled trees have the largest number of
 *          coalescence sequences.
 * https://oeis.org/A306390
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(1 + floor(log2((n-1)/3)))
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>

seq_t a(idx_t n)
{
    /* The sequence starts at offset 3, so n must be at least 3. */
    assert(n >= 3);

    /* Compute the integer m = floor((n-1)/3). */
    idx_t m = (n - 1) / 3;

    /* The special case n = 3 gives m = 0 and a(3) = 1. */
    if (m == 0)
        return (seq_t)1;

    /* Compute floor(log2(m)) by shifting. */
    unsigned k = 0;
    idx_t tmp = m;
    while ((tmp >>= 1) != 0)
        ++k;

    /* Result is 2^(k+1).  Compute it with seq_t arithmetic to avoid
       overflow problems on small seq_t types.  Multiplication on unsigned
       types is defined modulo wrap‑around. */
    seq_t result = (seq_t)1;
    for (unsigned i = 0; i < k + 1; ++i)
        result *= (seq_t)2;

    return result;
}
