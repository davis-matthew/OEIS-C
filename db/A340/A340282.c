/*
 * A340282  Number of partitions of n into 3 parts such that the largest part
 *          is relatively prime to at least one other part.
 * https://oeis.org/A340282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 parts (a ≤ b ≤ c,
 *            a+b+c=n) such that the largest part c is relatively prime to at
 *            least one of the other two parts. Computation is O(n²) with
 *            simple nested loops
 * Range:     u8 n <= 58, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t count = 0;

    /* loop over the smallest part a */
    for (idx_t a_part = 1; a_part <= n / 3; ++a_part) {
        /* b ranges from a_part up to floor((n - a_part)/2) */
        idx_t max_b = (n - a_part) / 2;
        for (idx_t b_part = a_part; b_part <= max_b; ++b_part) {
            idx_t c_part = n - a_part - b_part;   /* remaining part, automatically ≥ b_part */

            /* count only if c_part is coprime with at least one of the others */
            if (gcd((seq_t)c_part, (seq_t)a_part) == 1 ||
                gcd((seq_t)c_part, (seq_t)b_part) == 1) {
                ++count;
            }
        }
    }
    return count;
}
