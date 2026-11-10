/*
 * A302848  Permutation of natural numbers: a(1) = 1; for n > 1, a(n) =
 *          1+A064989(A003961(n)-2).
 * https://oeis.org/A302848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 1 + A064989(A003961(n) - 2)
 * Range:     u8 n <= 47, u16 n <= 1727, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 101 terms through n = 9381 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A302848_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A302848_next(uint64_t p)
{
    uint64_t q = p + 1;
    while (!A302848_isp(q)) q++;
    return q;
}

static uint64_t A302848_prev(uint64_t p)
{
    uint64_t q = p - 1;
    while (!A302848_isp(q)) q--;
    return q;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* A003961(n) */
    uint64_t m = (uint64_t)n;
    uint64_t s = 1;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            uint64_t q = A302848_next(p);
            while (m % p == 0) {
                m /= p;
                s *= q;
            }
        }
    }
    if (m > 1) s *= A302848_next(m);

    /* A064989(s - 2) */
    uint64_t t = s - 2;
    seq_t result = (seq_t)1;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t pv = A302848_prev(p);
            while (t % p == 0) {
                t /= p;
                result *= (seq_t)pv;
            }
        }
    }
    if (t > 1) result *= (seq_t)A302848_prev(t);

    return result + (seq_t)1;
}
