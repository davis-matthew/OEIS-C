/*
 * A366191  Enumeration of the rational numbers in the closed real interval
 *          [0, 1] after Cantor.
 * https://oeis.org/A366191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9795, u16 n <= 9795, u32 n <= 9795, u64 n <= 9795,
 *            u128 n <= 9795, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates the rational numbers in [0,1] by Cantor's method.
   The sequence a(n) is defined by:
     a(1) = 0, a(2) = 1.
   For k ≥ 1, let (p_k , q_k)  be the k‑th pair in the order
     first by increasing denominator n, then by increasing numerator t
     (t ≤ n/2) with gcd(t,n) = 1, where the pair is (t, n‑t).
   Then a(2k‑1) = p_k and a(2k) = q_k.
   Example: a = 0,1,1,1,1,2,1,3,1,4,2,3,1,5,1,6,2,5,3,4,1,7,3,5,...
*/

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* pair index determines which rational pair we are in */
    idx_t pair_idx = (n + 1) / 2;           /* ceil(n/2) */

    /* Handle the very first pair (0,1) separately */
    if (pair_idx == 1) {
        return (n & 1) ? (seq_t)0 : (seq_t)1;   /* odd -> numerator, even -> denominator */
    }

    /* Count all pairs up to the current denominator */
    idx_t cumulative = 1;            /* (0,1) has already been counted */
    for (idx_t denom = 2; ; ++denom) {
        idx_t half = denom / 2;
        idx_t block_cnt = 0;

        /* Count how many t in 1..floor(denom/2) satisfy gcd(t,denom)=1 */
        for (idx_t t = 1; t <= half; ++t) {
            if (gcd((seq_t)t, (seq_t)denom) == 1) {
                ++block_cnt;
            }
        }

        /* If the desired pair lies in this block, find it */
        if (cumulative + block_cnt >= pair_idx) {
            idx_t idx_within = pair_idx - cumulative - 1;   /* 0‑based index inside this block */
            idx_t t = 0;
            for (idx_t tt = 1; tt <= half; ++tt) {
                if (gcd((seq_t)tt, (seq_t)denom) == 1) {
                    if (idx_within == 0) {
                        t = tt;
                        break;
                    }
                    --idx_within;
                }
            }
            if (n & 1) {          /* odd n -> numerator */
                return (seq_t)t;
            } else {              /* even n -> denominator */
                return (seq_t)(denom - t);
            }
        }

        cumulative += block_cnt;    /* advance to next block */
    }

    /* Unreachable, but required for all control paths */
    return 0;
}
