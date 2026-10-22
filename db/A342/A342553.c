/*
 * A342553  Least integer m > 2*n such that m-2*n and m+2*n are both squares,
 *          for n>1.
 * https://oeis.org/A342553
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m > 2*n such that m-2*n and m+2*n are both squares
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;        /* offset starts at 2, value irrelevant otherwise */

    seq_t best = 0;
    bool found = false;

    /* iterate divisors a of n up to sqrt(n) */
    for (idx_t a = 1; ; ++a) {
        /* stop when a*a > n to avoid overflow using 128-bit */
        if ((unsigned __int128)a * a > (unsigned __int128)n) break;

        if (n % a == 0) {
            idx_t b = n / a;
            if (a == b) continue;   /* diff = 0 would give m = 2*n, too small */

            seq_t diff = (seq_t)b - (seq_t)a;   /* b > a so diff > 0 */
            seq_t sq   = diff * diff;
            seq_t m    = (seq_t)2 * (seq_t)n + sq;

            if (!found || m < best) {
                best = m;
                found = true;
            }
        }
    }

    return best;   /* always found for n >= 2 */
}
