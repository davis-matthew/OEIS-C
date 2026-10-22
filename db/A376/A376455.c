/*
 * A376455  a(n) = least k such that n^(2k+1)/(2k+1)! < 1.
 * https://oeis.org/A376455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that n^(2k+1)/(2k+1)! < 1 (computed via
 *            logarithmic comparison with lgamma)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   log_diff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/log_diff.h"

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == 0) return (seq_t)1;

    long double ln_n = logl((long double)n);

    idx_t low = 0;
    idx_t high = 1;

    /* Find an upper bound where the inequality holds */
    while (log_diff(high, ln_n) >= 0.0L)
        high <<= 1;

    /* Binary search for the first k with log_diff < 0 */
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        if (log_diff(mid, ln_n) < 0.0L)
            high = mid;
        else
            low = mid;
    }

    return (seq_t)high;
}
