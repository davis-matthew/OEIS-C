/*
 * A270832  Number of bases in which n is written using more than 1 digit, all
 *          of which are distinct.
 * https://oeis.org/A270832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 266, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A270832: number of bases b (2 ≤ b ≤ n) in which n is written
   using more than one digit, all of which are distinct. */

/* seq_t and idx_t are supplied by the build. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)        /* offset starts at 1; a(1)=0 */
        return 0;

    seq_t count = 0;

    /* loop over all admissible bases */
    for (idx_t b = 2; b <= n; ++b) {
        idx_t x = n;
        idx_t digits[70];   /* 70 > maximum number of digits for any 64‑bit n in base 2 */
        int len = 0;

        /* convert n to base b */
        while (x > 0) {
            digits[len++] = x % b;
            x /= b;
        }
        /* len >= 2 for all b ≤ n, but be safe */
        if (len <= 1)
            continue;

        /* check that all digits are distinct */
        bool distinct = true;
        for (int i = 0; i < len && distinct; ++i) {
            for (int j = i + 1; j < len; ++j) {
                if (digits[i] == digits[j]) {
                    distinct = false;
                    break;
                }
            }
        }
        if (distinct)
            ++count;
    }

    return count;
}
