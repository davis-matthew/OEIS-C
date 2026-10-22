/*
 * A339685  a(n) = Sum_{d|n} 5^(d-1).
 * https://oeis.org/A339685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 5^(d-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            sum += power5(i - 1);
            idx_t d2 = n / i;
            if (d2 != i) {
                sum += power5(d2 - 1);
            }
        }
    }
    return sum;
}
