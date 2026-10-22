/*
 * A328457  Length of the longest run of divisors > 1 of n.
 * https://oeis.org/A328457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the longest consecutive run of divisors > 1 of
 *            n
 * Range:     u8 n <= 8762, u16 n <= 8762, u32 n <= 8762, u64 n <= 8762,
 *            u128 n <= 8762, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;               /* n=1 has no divisors >1 */

    idx_t max_run = 0, run = 0;
    for (idx_t d = 2; d > 0 && d <= n; ++d) {
        if (n % d == 0)
            ++run;            /* extend current run */
        else {
            if (run > max_run)
                max_run = run;
            run = 0;          /* start new run */
        }
    }
    if (run > max_run)
        max_run = run;        /* last run may be longest */

    return (seq_t)max_run;
}
