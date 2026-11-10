/*
 * A364042  a(n) = 2^A329697(n) - A000120(n).
 * https://oeis.org/A364042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^A329697(n) - A000120(n); A329697 iterates k -> k -
 *            k/lpf(k) until a power of 2
 * Range:     u8 n <= 3956, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 113 terms through n = 8698 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a364042_lpf(uint64_t m)
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
    unsigned steps = 0;
    while (k > 1 && (k & (k - 1)) != 0) {
        uint64_t p = a364042_lpf(k);
        k -= k / p;
        steps++;
    }
    unsigned pc = 0;
    uint64_t t = (uint64_t)n;
    while (t) {
        pc += (unsigned)(t & 1ULL);
        t >>= 1;
    }
    seq_t r = (seq_t)1;
    r <<= steps;
    return r - (seq_t)pc;
}
