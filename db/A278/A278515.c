/*
 * A278515  Number of steps required to iterate map k -> A255131(k) when
 *          starting from k = (A000196(n)+1)^2 before n is reached, or 0 if n
 *          is not reached.
 * https://oeis.org/A278515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of k -> k - A002828(k) (A255131),
 *            started at k = (floor(sqrt(n))+1)^2, needed to hit n exactly; 0
 *            if the orbit skips n. Computed by direct iteration
 * Range:     u8 n <= 10200, u16 n <= 10200, u32 n <= 10200, u64 n <= 10200,
 *            u128 n <= 10200, big any n
 * Validated: standard, pass, 137 terms through n = 9824 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

static bool A255131_two_squares(uint64_t m)
{
    uint64_t p;
    while (m % 2 == 0) m /= 2;
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (p % 4 == 3 && (e & 1)) return false;
        }
    }
    if (m > 1 && m % 4 == 3) return false;
    return true;
}

static uint64_t A255131_minsq(uint64_t m)
{
    uint64_t r;
    if (m == 0) return 0;
    r = isqrt_u64(m);
    if (r * r == m) return 1;
    if (A255131_two_squares(m)) return 2;
    while (m % 4 == 0) m /= 4;
    if (m % 8 == 7) return 4;
    return 3;
}

seq_t a(idx_t n)
{
    uint64_t t = (uint64_t)n;
    uint64_t r = isqrt_u64(t) + 1;
    uint64_t k = r * r;
    uint64_t steps = 0;
    while (k > t) {
        k -= A255131_minsq(k);
        steps++;
    }
    if (k == t) return (seq_t)steps;
    return (seq_t)0;
}
