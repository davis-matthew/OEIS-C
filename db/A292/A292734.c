/*
 * A292734  Numbers in which 4 outnumbers all other digits together.
 * https://oeis.org/A292734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force enumeration of numbers in which 4 outnumbers all other
 * digits together.  a(n) is the nth positive integer whose decimal
 * representation satisfies:  #4  >  (#0 + #1 + … + #9 except the 4's).
 *
 * The function scans integers from 1 upward, counting those that meet
 * the condition, until the nth such number is reached.
 * It uses the provided helper digit_counts for counting digits
 * and standard arithmetic for incrementing values.
 */

#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    seq_t val = 1;
    idx_t  count = 0;
    unsigned char cnt[10];

    while (1) {
        /* Count digits of the current candidate */
        digit_counts(val, cnt);

        /* Compute number of 4's and other digits */
        unsigned f  = cnt[4];
        unsigned other = 0;
        for (int d = 0; d < 10; ++d) {
            if (d != 4) other += cnt[d];
        }

        if (f > other) {
            ++count;
            if (count == n)
                return val;
        }

        /* Next integer */
        val = val + 1;
    }
}
