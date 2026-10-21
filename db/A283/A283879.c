/*
 * A283879  An eventually quasilinear solution to Hofstadter's Q recurrence.
 * https://oeis.org/A283879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 if n == 1; 0 if n == 3; 3 if n == 4; otherwise, a(4k) =
 *            4k, a(4k+1) = 4k-1 = n-2, a(4k+2) = 1, a(4k+3) = 4
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 3) return (seq_t)0;
    if (n == 4) return (seq_t)3;

    switch (n % 4)
    {
        case 0: /* n = 4k, n >= 8 */
            return (seq_t)n;          /* a(4k) = 4k */
        case 1: /* n = 4k+1, n >= 5 */
            return (seq_t)(n - 2);    /* a(4k+1) = 4k-1 = n-2 */
        case 2:                         /* n = 4k+2, n >= 6 */
            return (seq_t)1;          /* a(4k+2) = 1 */
        case 3:                         /* n = 4k+3, n >= 7 */
            return (seq_t)4;          /* a(4k+3) = 4 */
    }
    return (seq_t)0; /* unreachable, keeps compiler happy */
}
