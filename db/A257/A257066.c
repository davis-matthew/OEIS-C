/*
 * A257066  Number of length 4 1..(n+1) arrays with every leading partial sum
 *          divisible by 2 or 3.
 * https://oeis.org/A257066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed by dynamic programming over residues modulo 6.
 *            Each of the 4 partial sums must be divisible by 2 or by 3, i.e.
 *            must lie in the set {0,2,3,4} modulo 6
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   residue_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external dependencies other than the standard headers needed for
   the integer types. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/residue_counts.h"

seq_t a(idx_t n)
{
    const idx_t N = n + 1U;            /* upper bound of each array entry */
    seq_t count[6];                    /* counts of residues modulo 6 */

    /* Pre‑compute the distribution of values modulo 6. */
    residue_counts(N, count);

    /* The four allowed residues for a prefix sum. */
    static const bool allowed[6] = { true,  false, true, true, true, false };

    /* DP arrays: dp_prev holds counts for the current length,
       dp_next holds the new counts after adding one element. */
    seq_t dp_prev[6] = {0};
    seq_t dp_next[6];

    dp_prev[0] = 1U; /* empty sum, not checked against the condition */

    /* Perform 4 transitions, one for each element of the array. */
    for (int step = 0; step < 4; ++step)
    {
        for (int i = 0; i < 6; ++i) dp_next[i] = 0U;

        for (int p = 0; p < 6; ++p)
        {
            seq_t base = dp_prev[p];
            if (base == 0U) continue;

            for (int rX = 0; rX < 6; ++rX)
            {
                seq_t cnt = count[rX];
                if (cnt == 0U) continue;

                int new_r = (p + rX) % 6;
                if (!allowed[new_r]) continue;

                dp_next[new_r] += base * cnt;
            }
        }

        /* Copy dp_next to dp_prev for the next iteration. */
        for (int i = 0; i < 6; ++i) dp_prev[i] = dp_next[i];
    }

    /* Sum all valid ending residues to obtain the total number of arrays. */
    seq_t result = 0U;
    for (int r = 0; r < 6; ++r)
        if (allowed[r]) result += dp_prev[r];

    return result;
}
