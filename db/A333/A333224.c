/*
 * A333224  Number of distinct positive consecutive subsequence-sums of the
 *          k-th composition in standard order.
 * https://oeis.org/A333224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct positive consecutive subsequence-sums
 *            of the k‑th composition in standard order (row k of A066099)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   count_distinct_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_distinct_sums.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* empty composition → no positive sums */

    /* Step 1: collect positions of 1-bits in the reversed binary expansion
       of n (positions are 1‑indexed). */
    idx_t pos[64];
    size_t t = 0;         /* number of 1-bits */
    idx_t bit = 0;
    idx_t k = n;
    while (k) {
        if (k & 1)
            pos[t++] = bit + 1;   /* positions are 1‑indexed */
        k >>= 1;
        ++bit;
    }

    if (t == 0)           /* should not happen for n>0, but for safety */
        return (seq_t)0;

    /* Step 2: build the composition parts.
       First part is the first position; remaining parts are differences.
       After constructing the raw differences, reverse to obtain the final
       composition per the standard order definition. */
    seq_t parts[64];
    parts[0] = pos[0];
    for (size_t i = 1; i < t; ++i)
        parts[i] = pos[i] - pos[i - 1];

    /* Reverse the parts array to match the final composition. */
    for (size_t i = 0; i < t / 2; ++i) {
        seq_t tmp = parts[i];
        parts[i] = parts[t - 1 - i];
        parts[t - 1 - i] = tmp;
    }

    /* Step 3: count distinct positive consecutive subsequence sums. */
    return count_distinct_sums(parts, t);
}
