/*
 * A268238  From the Fouriest transform of n: write n in that base b >= 4
 *          which maximizes the number of 4's; in case of a tie pick the
 *          smallest b; sequence gives the number of 4's.
 * https://oeis.org/A268238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code implements A268238: for a given n it tests all bases
   b ≥ 4 (up to n+1 when necessary), counts the number of digits equal
   to 4 in the base‑b representation of n, and returns the maximum
   count (choosing the smallest b in case of a tie). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t best = 0;
    idx_t best_base = 4;        /* default if no base with a 4 is found */

    /* For n==4, base 5 produces a single digit 4; need to include it.
       For n<4 we only need to consider base 4.  For n>4 we can stop
       at base n, because base n+1 would give the single digit n,
       which is not 4. */
    idx_t limit = n;
    if (n == 4)
        limit = 5;           /* consider bases 4 and 5 */
    else if (n < 4)
        limit = 4;           /* only base 4 is relevant */

    for (idx_t b = 4; b <= limit; ++b) {
        idx_t m = n;
        seq_t cnt = 0;
        while (m) {
            if ((m % b) == 4)
                ++cnt;
            m /= b;
        }
        if (cnt > best) {
            best = cnt;
            best_base = b;
        } else if (cnt == best && b < best_base) {
            best_base = b;
        }
    }

    return best;
}
