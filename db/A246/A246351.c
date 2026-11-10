/*
 * A246351  Numbers k such that A048673(k) < k.
 * https://oeis.org/A246351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A048673(k) < k, where A048673(k) =
 *            (A003961(k)+1)/2; found by a linear search over k
 * Range:     u8 n <= 117, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9238 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A246351_isprime(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A246351_nextprime(uint64_t p)
{
    p++;
    while (!A246351_isprime(p)) p++;
    return p;
}

/* true if (A003961(k)+1)/2 < k */
static bool A246351_less(uint64_t k)
{
    if (k == 1) return false;
    uint64_t limit = 2 * k;           /* need prod + 1 < 2k, i.e. prod < 2k-1 */
    uint64_t prod = 1;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m || m > 1; ) {
        if (p * p > m) p = m;         /* remaining m is prime */
        if (m % p == 0) {
            uint64_t np = A246351_nextprime(p);
            while (m % p == 0) {
                m /= p;
                prod *= np;
                if (prod >= limit) return false;
            }
        }
        if (m == 1) break;
        p = (p == 2) ? 3 : p + 2;
    }
    return (prod + 1) / 2 < k;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    uint64_t k = 0;
    while (count < (uint64_t)n) {
        k++;
        if (A246351_less(k)) count++;
    }
    return (seq_t)k;
}
