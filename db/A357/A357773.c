/*
 * A357773  Odd numbers with two zeros in their binary expansion.
 * https://oeis.org/A357773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^L – 2^i – 2^j – 1, where - L is the length of the
 *            binary expansion (L ≥ 4); - i and j (i > j ≥ 1) are the
 *            positions of the two zero bits (positions counted from 0 for the
 *            least significant bit); The terms are enumerated by increasing L
 *            and, for each L, by decreasing i and, within each i,...
 * Range:     u8 n <= 35, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return 2^e as a seq_t.  Works for built–in integer sizes and
   for arbitrary‑precision integer types. */
static seq_t pow2(idx_t e)
{
    seq_t r = (seq_t)1;
    while (e) {
        r *= (seq_t)2;
        e--;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* offset is 1 */

    /* Find the bit‑length L that contains the n‑th term. */
    idx_t L = 4;            /* smallest length that can have two zeros */
    idx_t cum = 0;          /* terms counted so far (till previous L) */
    for (;;) {
        idx_t t = (L - 2) * (L - 3) / 2;   /* terms with exactly L bits */
        if (cum + t >= n)
            break;
        cum += t;
        L++;
    }

    /* Position of the term within the group of length L (1‑based). */
    idx_t idx = n - cum;               /* 1 ≤ idx ≤ t(L) */

    /* Determine the zero positions i and j.
       Iterate i from L-2 down to 2; each i contributes (i-1) terms. */
    idx_t i;
    for (i = L - 2; ; --i) {
        idx_t block = i - 1;
        if (idx > block)
            idx -= block;            /* skip this block */
        else
            break;                   /* found i */
    }

    /* Within the chosen block, j decreases from i-1 to 1. */
    idx_t j = i - idx;                 /* 1‑based mapping */

    /* Compute the value: 2^L – 2^i – 2^j – 1 */
    return pow2(L) - pow2(i) - pow2(j) - (seq_t)1;
}
