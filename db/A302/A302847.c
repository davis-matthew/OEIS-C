/*
 * A302847  Permutation of natural numbers: a(1) = 1; for n > 1, a(n) =
 *          A064989(2+A003961(n-1)).
 * https://oeis.org/A302847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = A064989(2+A003961(n-1))
 * Range:     u8 n <= 60, u16 n <= 1728, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 102 terms through n = 9716 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A302847_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A302847_next(uint64_t p)
{
    uint64_t q = p + 1;
    while (!A302847_isp(q)) q++;
    return q;
}

static uint64_t A302847_prev(uint64_t p)
{
    uint64_t q = p - 1;
    while (!A302847_isp(q)) q--;
    return q;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* A003961(n-1) */
    uint64_t m = (uint64_t)(n - 1);
    uint64_t s = 1;
    for (uint64_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            uint64_t q = A302847_next(p);
            while (m % p == 0) {
                m /= p;
                s *= q;
            }
        }
    }
    if (m > 1) s *= A302847_next(m);

    /* A064989(2 + s); 2 + s is odd */
    uint64_t t = s + 2;
    seq_t result = (seq_t)1;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t q = A302847_prev(p);
            while (t % p == 0) {
                t /= p;
                result *= (seq_t)q;
            }
        }
    }
    if (t > 1) result *= (seq_t)A302847_prev(t);
    return result;
}
