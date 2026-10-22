/*
 * A359183  a(n) is the smallest number such that when written in all bases
 *          from base 2 to base n its leading digit equals the base - 1.
 * https://oeis.org/A359183
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest positive integer such that for every base b
 *            (2 ≤ b ≤ n) the leading digit of a in base b equals b – 1. The
 *            algorithm iteratively computes a fixed point of the monotone
 *            function a → max_{2≤b≤n} (b – 1)·b^{⌊log_b a⌋}
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2; values below 2 are irrelevant. */
    if (n < 2) return (seq_t)0;

    const idx_t maxb = n;

    /* Allocate an array to hold the largest power of each base not exceeding
       the current candidate a.  We index from 0 to maxb; entries 0 and 1
       are unused. */
    seq_t *pow = seq_alloc_new_array(maxb + 1);
    for (idx_t b = 2; b <= maxb; ++b) {
        pow[b] = (seq_t)1;          /* b^0 */
    }

    seq_t a_val = (seq_t)1;          /* current candidate value */

    while (1) {
        seq_t m = (seq_t)0;          /* upper bound produced by all bases */

        for (idx_t b = 2; b <= maxb; ++b) {
            seq_t bb = (seq_t)b;

            /* Update pow[b] to the greatest power of b that does not
               exceed the current candidate a_val.  The division is
               safe for 64‑bit arithmetic and works for arbitrary
               precision as well. */
            while (pow[b] <= a_val / bb) {
                pow[b] *= bb;
            }

            /* Candidate that satisfies base b:  (b‑1) · b^{floor_log_b a_val} */
            seq_t candidate = pow[b] * ((seq_t)(b - 1));
            if (candidate > m) {
                m = candidate;
            }
        }

        /* If the current candidate a_val already satisfies every base,
           we have reached the fixed point.  Otherwise set a_val to the
           new upper bound and repeat. */
        if (m <= a_val) {
            break;
        }
        a_val = m;
    }

    seq_free_array(pow);
    return a_val;
}
