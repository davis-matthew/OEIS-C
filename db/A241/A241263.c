/*
 * A241263  Successive break values in reaching a maximum break of 147 in
 *          snooker.
 * https://oeis.org/A241263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = 7*(n/2)+((n+1)/2) for 1 <= n <= 30 a(n) = 120 + sum of
 *            first (n-30) of {2,3,4,5,6,7} for 30 < n <= 36 a(n) = 147 for n
 *            > 36
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t color_vals[6] = {2, 3, 4, 5, 6, 7};

seq_t a(idx_t n)
{
    if (n <= 30) {
        seq_t sn = (seq_t)n;
        return (seq_t)7 * (sn / 2) + ((sn + 1) / 2);
    }
    if (n <= 36) {
        seq_t sum = (seq_t)120;
        size_t m = (size_t)(n - 30); /* 1..6 */
        for (size_t i = 0; i < m; ++i) {
            sum += color_vals[i];
        }
        return sum;
    }
    return (seq_t)147;
}
