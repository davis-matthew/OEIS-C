/*
 * A292452  Numbers where 2 outnumbers any other digit.
 * https://oeis.org/A292452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers where digit 2 outnumbers any other digit. */
#include <stdbool.h>
#include "../utils/digit_counts.h"

seq_t a(idx_t n)
{
    seq_t val = 2;              /* first positive integer containing a 2 */
    idx_t seen = 0;             /* how many qualifying numbers we have found */

    while (1) {
        unsigned char cnt[10] = {0};
        digit_counts(val, cnt);

        unsigned char c2 = cnt[2];
        bool ok = true;
        for (int d = 0; d < 10 && ok; ++d) {
            if (d == 2) continue;
            if (c2 <= cnt[d]) ok = false;
        }

        if (ok) {
            ++seen;
            if (seen == n) return val;
        }

        /* increment to the next integer */
        val = val + 1u;
    }
}
