/*
 * A359583  Parity of A329697.
 * https://oeis.org/A359583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A329697(n) mod 2, where A329697(n) = 0 if n is a power of
 *            2, else 1 + A329697(n - n/LPF(n)), LPF = largest prime factor
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9981 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A359583_lpf(uint64_t m)
{
    uint64_t largest = 1;
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)
        largest = m;
    return largest;
}

seq_t a(idx_t n)
{
    uint64_t k = (uint64_t)n;
    unsigned cnt = 0;
    while (k > 1) {
        while ((k & 1ULL) == 0)
            k >>= 1;
        if (k == 1)
            break;
        uint64_t p = A359583_lpf(k);
        k -= k / p;
        cnt++;
    }
    return (seq_t)(cnt & 1u);
}
