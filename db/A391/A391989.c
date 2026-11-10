/*
 * A391989  The difference between the number of exponential divisors and the
 *          number of exponential unitary divisors for numbers that are not
 *          exponentially squarefree.
 * https://oeis.org/A391989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A049419(A130897(n)) - A278908(A130897(n)); search over k
 *            >= 2 for numbers with a non-squarefree exponent, and for each
 *            one compute Product tau(e) - Product 2^omega(e) over p^e || k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9336 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A391989_sqfree(unsigned e)
{
    for (unsigned d = 2; d * d <= e; d++)
        if (e % (d * d) == 0) return false;
    return true;
}

static unsigned A391989_tau(unsigned e)
{
    unsigned c = 0;
    for (unsigned d = 1; d <= e; d++)
        if (e % d == 0) c++;
    return c;
}

static unsigned A391989_pow2omega(unsigned e)
{
    unsigned r = 1;
    for (unsigned d = 2; d <= e; d++) {
        if (e % d == 0) {
            r *= 2;
            while (e % d == 0) e /= d;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (uint64_t m = 2;; ++m) {
        uint64_t x = m;
        bool bad = false;
        seq_t t = 1, u = 1;
        for (uint64_t p = 2; p * p <= x; ++p) {
            if (x % p == 0) {
                unsigned e = 0;
                while (x % p == 0) { x /= p; ++e; }
                if (!A391989_sqfree(e)) bad = true;
                t *= (seq_t)A391989_tau(e);
                u *= (seq_t)A391989_pow2omega(e);
            }
        }
        /* a leftover prime has exponent 1: tau(1) = 1 and 2^omega(1) = 1 */
        if (bad) {
            ++count;
            if (count == n) return t - u;
        }
    }
}
