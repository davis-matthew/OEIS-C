/*
 * A334091  a(1) = 0, then after the first differences of A329697.
 * https://oeis.org/A334091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 0, a(n) = A329697(n) - A329697(n-1) for n > 1, where
 *            A329697(n) counts iterations of k -> k - k/lpf(k) (lpf = largest
 *            prime factor) needed to reach a power of 2
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9169 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static uint64_t A334091_lpd(uint64_t m)
{
    uint64_t largest = 0;
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

static long long A334091_A329697(uint64_t n)
{
    long long count = 0;
    while (n > 2 && (n & (n - 1)) != 0) {
        while ((n & 1ULL) == 0)
            n >>= 1;
        uint64_t p = A334091_lpd(n);
        n = n - n / p;
        count++;
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    long long d = A334091_A329697((uint64_t)n) - A334091_A329697((uint64_t)n - 1);
    return (seq_t)d;
}
