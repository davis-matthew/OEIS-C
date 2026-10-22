/*
 * A335478  Numbers k such that the k-th composition in standard order
 *          (A066099) matches the pattern (2,1,1).
 * https://oeis.org/A335478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Enumerates all compositions via the binary representation of k.
   For each k with at least 3 parts, builds the composition in the
   standard order (A066099).  It then tests whether the composition
   contains a subsequence whose parts have the same relative order as
   the pattern (2,1,1).  The function returns the nth integer k whose
   composition satisfies the pattern. */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* offset is 1, unused for n=0 */
    idx_t count = 0;
    idx_t k = 1;
    while (1) {
        /* Fast popcount to skip compositions with fewer than 3 parts */
        idx_t tmp = k;
        int ones = 0;
        while (tmp) {
            tmp &= tmp - 1;
            ++ones;
        }
        if (ones >= 3) {
            /* Build parts in reverse order (diffs between 1‑bit positions) */
            static idx_t rev_parts[128];   /* holds parts_rev */
            static idx_t comp[128];        /* final composition */
            idx_t pos = 0, last = 0;
            idx_t tmp2 = k;
            int m = 0;
            while (tmp2) {
                if (tmp2 & 1) {
                    rev_parts[m++] = (pos + 1) - last;
                    last = pos + 1;
                }
                ++pos;
                tmp2 >>= 1;
            }
            /* Reverse to get the standard composition order */
            for (int t = 0; t < m; ++t)
                comp[t] = rev_parts[m - 1 - t];

            /* Test for pattern (2,1,1): must find i<j<k with
               comp[i] > comp[j] && comp[j] == comp[k] */
            bool found = false;
            for (int i = 0; i < m - 1 && !found; ++i) {
                for (int j = i + 1; j < m - 1 && !found; ++j) {
                    if (comp[i] > comp[j]) {
                        for (int k2 = j + 1; k2 < m; ++k2) {
                            if (comp[j] == comp[k2]) {
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (found) {
                ++count;
                if (count == n)
                    return (seq_t)k;
            }
        }
        ++k;
    }
}
