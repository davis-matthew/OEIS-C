/*
 * A275910  Numbers not congruent to 0, 1 or 8 mod 9.
 * https://oeis.org/A275910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9 * ((n-1)/6) + {2,3,4,5,6,7}[(n-1)%6] (n ≥ 1)
 * Range:     u8 n <= 170, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n == 0 return 0 as a safe fallback. */
    if (n == 0)
        return 0;

    idx_t m = n - 1;          /* zero‑based index */
    idx_t q = m / 6;          /* complete blocks of 6 valid numbers */
    idx_t r = m % 6;          /* position inside the current block (0..5) */

    static const seq_t residues[6] = {2, 3, 4, 5, 6, 7};

    /* 9 * q gives the starting number of the block, then add the residue. */
    seq_t base = (seq_t)q * (seq_t)9;
    return base + residues[(size_t)r];
}
