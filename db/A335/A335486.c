/*
 * A335486  Numbers k such that the k-th composition in standard order
 *          (A066099) is not weakly increasing.
 * https://oeis.org/A335486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th k such that the k-th composition (graded reverse‑lexicographic)
   is not weakly increasing.  The implementation iterates over sums s, counts
   the non‑weak compositions of s, locates the correct block, and scans the
   compositions of that sum to find the required k. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Pre‑compute partition numbers p(m) for m up to 200.
       p(0)=1; for m>0, use the pentagonal number recurrence. */
    static idx_t part[201];
    static bool init = false;
    if (!init) {
        part[0] = 1;
        for (int m = 1; m <= 200; ++m) {
            idx_t sum = 0;
            for (int k = 1; ; ++k) {
                int pent1 = k * (3 * k - 1) / 2;
                if (pent1 > m) break;
                int sign = (k & 1) ? 1 : -1;
                sum += (idx_t)sign * part[m - pent1];
                int pent2 = k * (3 * k + 1) / 2;
                if (pent2 > m) continue;
                sum += (idx_t)sign * part[m - pent2];
            }
            part[m] = sum;
        }
        init = true;
    }

    idx_t target = n;          /* 1‑based target index */
    idx_t cumulative = 0;      /* number of non‑weak compositions seen so far */
    idx_t s = 1;               /* current sum (composition length) */

    while (1) {
        /* Reject sums that would overflow the shift. */
        if (s >= 65) return (seq_t)0;    /* safe guard – not needed for usual n */

        idx_t compCnt = (idx_t)1 << (s - 1);          /* 2^(s‑1) compositions of sum s */
        idx_t weakCnt = part[s];                    /* partitions of s → weakly increasing compositions */
        idx_t nonWeak  = compCnt - weakCnt;

        if (target <= cumulative + nonWeak) {
            /* Desired k lies in the block of sum s. */
            idx_t rank = target - cumulative;   /* 1‑based rank within this block */
            idx_t startK = (idx_t)1 << (s - 1);
            idx_t found = 0;

            for (idx_t t = 0; t < compCnt; ++t) {
                idx_t k = startK + t;

                /* Build the parts of the composition from the bits of k.
                   parts[0…depth-1] holds the forward differences. */
                int depth = 0;
                int last = 0;
                idx_t tmp = k;
                uint32_t parts[128];   /* s ≤ 64 → depth ≤ 64 */

                for (int pos = 1; pos <= (int)s - 1; ++pos) {
                    if (tmp & 1) {
                        parts[depth++] = (uint32_t)(pos - last);
                        last = pos;
                    }
                    tmp >>= 1;
                }
                parts[depth++] = (uint32_t)(s - last);

                /* Check whether the reversed composition is weakly increasing.
                   That is equivalent to checking whether parts[0…depth-1]
                   is non‑increasing. */
                bool weak = true;
                for (int i = 0; i < depth - 1; ++i) {
                    if (parts[i] < parts[i + 1]) { weak = false; break; }
                }
                if (!weak) {   /* non‑weak composition */
                    ++found;
                    if (found == rank) {
                        return (seq_t)k;
                    }
                }
            }
            /* Should never get here. */
            return (seq_t)0;
        } else {
            cumulative += nonWeak;
            ++s;
        }
    }
}
