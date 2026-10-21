/*
 * A241426  Value of concatenation of all suffixes of binary representation of
 *          n.
 * https://oeis.org/A241426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{L=1}^{k(n)} ((n mod 2^L) << (L*(L-1)/2)) where k(n) is
 *            the length (number of bits) of n's binary expansion. The sum is
 *            over the suffixes of the binary representation of n,
 *            concatenated from longest to shortest
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 127, u64 n <= 1023,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Determine the number of bits k of n (k ≥ 1) */
    uint64_t temp = (uint64_t)n;
    idx_t k = 0;
    while (temp) { ++k; temp >>= 1; }

    seq_t res = 0;
    const idx_t shift_cap = sizeof(seq_t) * 8;   /* maximum safe shift */

    for (idx_t L = 1; L <= k; ++L)
    {
        /* Compute mask for the lowest L bits */
        seq_t mask;
        if (L == shift_cap)        /* avoid shifting by the word size */
            mask = ~(seq_t)0;
        else
            mask = (((seq_t)1) << (unsigned long long)L) - 1;

        seq_t val = ((seq_t)n) & mask;          /* n mod 2^L */

        idx_t shift = L * (L - 1) / 2;           /* total bits of suffixes that follow */

        if (shift < shift_cap)
            res += val << (unsigned long long)shift;
        /* If shift ≥ shift_cap the term cannot be represented in seq_t;
           it is omitted because the full value would not fit in seq_t. */
    }

    return res;
}
