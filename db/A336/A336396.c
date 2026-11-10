/*
 * A336396  a(n) = A329697(n) - A087436(n).
 * https://oeis.org/A336396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A329697(n) - A087436(n); A329697 by iterating k -> k -
 *            k/lpf(k) until k is a power of 2, A087436 = number of odd prime
 *            factors with multiplicity
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8736 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A336396_lpf(uint64_t m)
{
    uint64_t largest = 1;
    while ((m & 1ULL) == 0) { largest = 2; m >>= 1; }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0) m /= f;
        }
    }
    if (m > 1) largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n;
    if (k <= 1) return (seq_t)0;

    /* A087436: odd prime factors with multiplicity */
    uint64_t m = k;
    uint64_t cnt = 0;
    while ((m & 1ULL) == 0) m >>= 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) { m /= p; ++cnt; }
    }
    if (m > 1) ++cnt;

    /* A329697: iteration count */
    uint64_t steps = 0;
    while ((k & (k - 1)) != 0) {
        uint64_t p = A336396_lpf(k);
        k -= k / p;
        ++steps;
    }

    return (seq_t)(steps - cnt);
}
