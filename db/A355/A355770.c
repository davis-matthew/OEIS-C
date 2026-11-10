/*
 * A355770  a(n) is the number of terms of A333369 that divide n.
 * https://oeis.org/A355770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n} [d is in A333369]; computed by enumerating
 *            divisors of n up to sqrt(n) and testing each one's digit-count
 *            parities
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A333369_ok(uint64_t x)
{
    unsigned int freq[10] = {0};
    if (x == 0) return false;
    while (x > 0) {
        freq[x % 10]++;
        x /= 10;
    }
    for (int d = 0; d < 10; ++d) {
        if (freq[d] == 0) continue;
        if ((d & 1) == 0) {
            if (freq[d] & 1) return false;
        } else {
            if (!(freq[d] & 1)) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t cnt = 0;
    uint64_t d;
    if (m == 0) return (seq_t)0;
    for (d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            uint64_t e = m / d;
            if (A333369_ok(d)) ++cnt;
            if (e != d && A333369_ok(e)) ++cnt;
        }
    }
    return (seq_t)cnt;
}
