/*
 * A339685  a(n) = Sum_{d|n} 5^(d-1).
 * https://oeis.org/A339685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 5^(d-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 990 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

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
