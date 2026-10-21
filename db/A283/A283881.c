/*
 * A283881  A linear-recurrent solution to Hofstadter's Q recurrence.
 * https://oeis.org/A283881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(7n) = 4 a(7n+1) = 7 a(7n+2) = 7n a(7n+3) = 8n+8 a(7n+4) = 7
 *            a(7n+5) = 7 a(7n+6) = 2^(n+3)
 * Range:     u8 n <= 40, u16 n <= 96, u32 n <= 208, u64 n <= 432,
 *            u128 n <= 880, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the term using the piecewise formulas. */
    switch (n % 7) {
        case 0:                 /* a(7k) = 4 */
            return 4;

        case 1:                 /* a(7k+1) = 7 */
            return 7;

        case 2: {               /* a(7k+2) = 7k */
            idx_t k = (n - 2) / 7;
            return (seq_t)7 * (seq_t)k;
        }

        case 3: {               /* a(7k+3) = 8k + 8 */
            idx_t k = (n - 3) / 7;
            return (seq_t)8 * (seq_t)k + (seq_t)8;
        }

        case 4:                 /* a(7k+4) = 7 */
            return 7;

        case 5:                 /* a(7k+5) = 7 */
            return 7;

        case 6: {               /* a(7k+6) = 2^(k+3) */
            idx_t k = (n - 6) / 7;
            return pow2(k + 3);
        }

        default:                /* unreachable for n >= 1 */
            return 0;
    }
}
