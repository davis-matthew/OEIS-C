/*
 * A335877  a(n) = A331410(n) - A329697(n).
 * https://oeis.org/A335877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A331410(n) - A329697(n); A331410 iterates k -> k +
 *            k/lpf(k), A329697 iterates k -> k - k/lpf(k), each counting
 *            steps until a power of 2 is reached (computed by direct
 *            iteration)
 * Range:     i8 n <= 65537, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9615 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static uint64_t A335877_lpf(uint64_t m)
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

static bool A335877_pow2(uint64_t k)
{
    return k != 0 && (k & (k - 1)) == 0;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    long long up = 0, down = 0;

    uint64_t k = (uint64_t)n;
    while (!A335877_pow2(k)) {
        uint64_t p = A335877_lpf(k);
        k += k / p;
        ++up;
    }

    k = (uint64_t)n;
    while (!A335877_pow2(k)) {
        uint64_t p = A335877_lpf(k);
        k -= k / p;
        ++down;
    }

    return (seq_t)(up - down);
}
