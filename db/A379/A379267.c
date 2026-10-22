/*
 * A379267  Numbers whose binary representation contains exactly two zeros.
 * https://oeis.org/A379267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) computed as 2^L – 1 – 2^i – 2^j, where L is the smallest
 *            integer with (L‑1)L(L‑2)/6 ≥ n. The pair (i,j) (i>j≥0) is chosen
 *            by the combinatorial ordering of binary strings with exactly two
 *            zeros
 * Range:     u8 n <= 56, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1; harmless */

    /* Find the bit‑length L such that the cumulative count ≥ n. */
    idx_t L = 3;
    while (((L - 1) * L * (L - 2) / 6) < n)
        ++L;

    /* Count of terms with length < L. */
    idx_t cum_prev = ((L - 2) * (L - 1) * (L - 3) / 6);
    idx_t offset   = n - cum_prev - 1;   /* zero‑based index within length L */

    /* Locate the positions of the two zeros. */
    idx_t i = L - 2;
    idx_t cumulative = 0;
    while (1) {
        idx_t count_i = i;                 /* j can be 0 … i‑1 */
        if (offset < cumulative + count_i) {
            idx_t offset_within = offset - cumulative;
            idx_t j = i - 1 - offset_within;

            /* Construct the number: 2^L – 1 – 2^i – 2^j. */
            seq_t val = pow2(L);
            val -= 1;
            val -= pow2(i);
            val -= pow2(j);
            return val;
        }
        cumulative += count_i;
        if (i == 1) break;                /* safety check */
        --i;
    }

    return (seq_t)0;   /* unreachable for valid n */
}
