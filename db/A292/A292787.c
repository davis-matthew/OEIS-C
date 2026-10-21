/*
 * A292787  For n > 1, a(n) = least positive k, not a power of n, such that
 *          the digital sum of k in base n equals the digital sum of k^2 in
 *          base n.
 * https://oeis.org/A292787
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k > 0, k not a power of n, such that digit_sum_n(k)
 *            = digit_sum_n(k^2) (digit sums taken in base‑n)
 * Range:     u8 n <= 16, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_power_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_power_of_n.h"

static seq_t digit_sum(seq_t x, seq_t base) {
    seq_t sum = 0;
    while (x > 0) {
        sum += x % base;
        x /= base;
    }
    return sum;
}

seq_t a(idx_t n) {
    /* Special case: n must be at least 2 per the definition. */
    if (n < 2) return 0; /* Undefined for n < 2 */

    seq_t base = (seq_t)n;
    seq_t k = 1;

    for (;; ++k) {
        /* Skip powers of n (including 1). */
        if (is_power_of_n(base, k)) continue;

        seq_t sum1 = digit_sum(k, base);
        seq_t sq = k * k;
        seq_t sum2 = digit_sum(sq, base);

        if (sum1 == sum2) return k;
    }
}
