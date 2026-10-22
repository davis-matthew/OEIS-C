/*
 * A355366  Maximal GCD of five positive integers with sum n.
 * https://oeis.org/A355366
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ d | d divides n and d <= n/5 }
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 5) return 0;            /* offset starts at n = 5 */
    idx_t thresh = n / 5;           /* integer division */
    seq_t best = 1;                 /* at least 1 divides every n >=5 */

    for (idx_t d = 1; d <= thresh; ++d) {
        if (((seq_t)n) % d == 0) {  /* d is a divisor of n */
            if ((seq_t)d > best) {
                best = (seq_t)d;
            }
        }
    }
    return best;
}
