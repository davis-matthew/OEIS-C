/*
 * A348951  a(n) = Sum_{d|n, d < sqrt(n)} (-1)^(n/d).
 * https://oeis.org/A348951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n, d < sqrt(n)} (-1)^(n/d)
 * Range:     i8 n <= 9964, i16 n <= 9964, i32 n <= 9964, i64 n <= 9964,
 *            i128 n <= 9964, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer floor square root of an unsigned 64‑bit integer */
static idx_t floor_isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;          /* safeguard, though n ≥ 1 by contract */

    const idx_t s = floor_isqrt_u64(n);

    for (idx_t d = 1; d <= s; ++d) {
        if (n % d == 0) {
            /* skip the divisor equal to sqrt(n) in the perfect‑square case */
            if (n / d == d)
                continue;

            /* add +1 if (n/d) is even, else add -1 */
            sum += ((n / d) & 1u) ? -1 : 1;
        }
    }
    return sum;
}
