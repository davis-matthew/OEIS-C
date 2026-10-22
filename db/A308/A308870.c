/*
 * A308870  Sum of the fourth largest parts in the partitions of n into 6
 *          parts.
 * https://oeis.org/A308870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over k≥1 of k × (# of partitions of n into 6 parts
 *            with the 4th (largest) part equal to k)
 * Range:     u8 n <= 21, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   p3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented via a short combinatorial formula that runs in O(n²) time
   (≈2.8 million loop iterations for n = 10000). */

#include <stdint.h>
#include "../utils/p3.h"

seq_t a(idx_t n)
{
    if (n < 6) return 0;

    seq_t result = 0;
    /* maximum possible value of the 4th part:
       4k + 1 + 1 ≤ n  ⇒  k ≤ (n‑2)/4. */
    idx_t maxk = (n - 2) / 4;

    for (idx_t k = 1; k <= maxk; ++k) {
        /* signed difference to allow negative values (when n < 6·k). */
        int64_t t = (int64_t)n - 6 * (int64_t)k;         /* t = n – 6k       */
        idx_t v_min = (t < 0) ? (idx_t)(-t) : 0;         /* lower bound for v   */
        idx_t v_max = 2 * k - 2;                        /* upper bound for y‐sum */

        seq_t total_for_k = 0;

        for (idx_t v = v_min; v <= v_max; ++v) {
            /* Count pairs (y1, y2) with y1 ≤ y2, y1 + y2 = v,
               and 0 ≤ y1, y2 ≤ k‑1. */
            idx_t lower = (v > (k - 1)) ? (v - (k - 1)) : 0;
            idx_t upper = v / 2;
            if (upper > (k - 1)) upper = k - 1;

            if (upper >= lower) {
                seq_t cnt_y = (seq_t)(upper - lower + 1);

                int64_t sum_b = t + (int64_t)v;          /* b = t + v ≥ 0   */
                seq_t p3_val   = p3((seq_t)sum_b);

                total_for_k += cnt_y * p3_val;
            }
        }
        result += (seq_t)k * total_for_k;
    }
    return result;
}
