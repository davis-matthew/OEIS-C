/*
 * A360525  Numbers k such that A360522(k) > 2*k.
 * https://oeis.org/A360525
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A360522(k) > 2*k, where A360522(k) =
 *            Product_{p^e || k} (p^e + e); found by search
 * Range:     u8 n <= 27, u16 n <= 7157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9923 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A360522_val(uint64_t m)
{
    uint64_t result = 1;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            uint64_t pe = 1, e = 0;
            while (m % p == 0) {
                m /= p;
                pe *= p;
                e++;
            }
            result *= (pe + e);
        }
    }
    if (m > 1)
        result *= (m + 1);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    if (n == 0)
        return (seq_t)0;
    while (count < (uint64_t)n) {
        k++;
        if (A360522_val(k) > 2 * k)
            count++;
    }
    return (seq_t)k;
}
