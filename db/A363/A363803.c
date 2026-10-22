/*
 * A363803  a(n) = Sum_{d|n, d == 0, 2, or 5 mod 7} d.
 * https://oeis.org/A363803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = Sum_{d|n, d ≡ 0,2,5 (mod 7)} d
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/good_mod.h"

/* integer square root: floor(sqrt(n)) without overflow */
static inline idx_t floor_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            low = 1;
            continue;
        }
        if (mid <= n / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* offset is 1; this case is unused */

    seq_t sum = 0;
    idx_t m    = floor_sqrt(n);

    for (idx_t i = 1; i <= m; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (good_mod(d1))
                sum += (seq_t)d1;

            if (d1 != d2 && good_mod(d2))
                sum += (seq_t)d2;
        }
    }
    return sum;
}
