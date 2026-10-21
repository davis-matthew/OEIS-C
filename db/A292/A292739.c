/*
 * A292739  Numbers in which 9 outnumbers all other digits together.
 * https://oeis.org/A292739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers in which the digit 9 outnumbers all other digits together.
   a(n) is the nth positive integer satisfying that property. */

#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;   /* undefined below offset, but return 0 */
    }

    seq_t cur = 0;          /* candidate number */
    idx_t found = 0;        /* count of valid numbers seen */
    unsigned char cnt[10];

    while (1) {
        cur += (seq_t)1;
        digit_counts(cur, cnt);

        unsigned int other = 0;
        for (int d = 0; d < 9; ++d) {
            other += cnt[d];
        }

        if (cnt[9] > other) {      /* 9's outnumber all other digits */
            ++found;
            if (found == n) {
                return cur;
            }
        }
    }
}
