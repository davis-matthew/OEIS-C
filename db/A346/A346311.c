/*
 * A346311  Maximum number of edges a single edge crosses in a drawing of the
 *          complete graph K_n where every vertex lies on the outer face.
 * https://oeis.org/A346311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = floor((n-2)^2/4) for n%4 ≠ 0; * for n%4 = 0 (n ≥ 8) a(n)
 *            = floor((n-2)^2/4) + (-1)^{(n/4)+1}
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 3)
        return 0;
    if (n == 4)
        return 1;

    idx_t m = n - 2;                     /* n-2 ≥ 2 for n ≥ 4 */
    seq_t base = (seq_t)m * (seq_t)m / 4;/* floor((n-2)^2/4) */

    if ((n % 4) == 0 && n >= 8) {
        /* Determine whether to add or subtract 1.
         * (-1)^{(n/4)+1} yields +1 when (n/4)+1 is even,
         * -1 when it is odd. */
        idx_t exp = (n / 4) + 1;
        if ((exp & 1) == 0) /* exp even → +1 */
            return base + 1;
        else                /* exp odd  → -1 */
            return base - 1;
    }

    return base;
}
