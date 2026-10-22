/*
 * A395409  a(n) = A033677(n) - A135034(n); excess of the smallest divisor of
 *          n >= sqrt(n) over ceiling(sqrt(n)).
 * https://oeis.org/A395409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min_{d|n} max(d, n/d) – ceil(sqrt(n)) (the smallest
 *            divisor of n that is ≥ sqrt(n), minus ceil(sqrt(n)))
 * Range:     u8 n <= 276, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>      /* for uint64_t and related types */

/* Compute floor(sqrt(n)) for a 64‑bit unsigned integer,
   using only integer arithmetic. */
static idx_t isqrt_floor(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;                      /* sqrt(n) ≤ n for n > 0 */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);  /* ceil mid */
        if (mid <= n / mid) {
            low = mid;                     /* mid is ≤ sqrt(n) */
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }

    /* ceil_sqrt = floor_sqrt if n is a perfect square, otherwise floor_sqrt + 1 */
    idx_t fs = isqrt_floor(n);
    idx_t ceil_sqrt = fs;
    if (fs * fs < n) {
        ceil_sqrt = fs + 1;
    }

    /* Find the smallest divisor d ≥ ceil_sqrt */
    idx_t d = ceil_sqrt;
    while (d <= n) {
        if (n % d == 0) {
            break;
        }
        ++d;
    }

    /* Return the difference in the type seq_t */
    return (seq_t)d - (seq_t)ceil_sqrt;
}
