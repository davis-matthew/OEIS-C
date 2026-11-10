/*
 * A253889  a(n) = A048673(floor(A064216(n)/2)).
 * https://oeis.org/A253889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048673(floor(A064216(n)/2)); A064216(n) = A064989(2n-1)
 *            replaces each p^e by prevprime(p)^e; A048673(k) =
 *            (A003961(k)+1)/2. a(1)=1 (special case)
 * Range:     u8 n <= 50, u16 n <= 2384, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 100 terms through n = 8064 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static bool a253889_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t a253889_prevp(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 2 && !a253889_isp(q)) --q;
    return q < 2 ? 2 : q;
}

static uint64_t a253889_nextp(uint64_t p)
{
    uint64_t q = p + 1;
    while (!a253889_isp(q)) ++q;
    return q;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    /* m = A064216(n): fits in uint64 (m <= 2n-1) */
    uint64_t t = 2ULL * (uint64_t)n - 1ULL;
    uint64_t m = 1;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t pr = a253889_prevp(p);
            while (t % p == 0) {
                t /= p;
                m *= pr;
            }
        }
    }
    if (t > 1) m *= a253889_prevp(t);

    /* A048673(floor(m/2)) */
    uint64_t k = m / 2;
    if (k <= 1) return (seq_t)1;

    seq_t prod = (seq_t)1;
    uint64_t r = k;
    for (uint64_t p = 2; p <= r / p; p += (p == 2 ? 1 : 2)) {
        if (r % p == 0) {
            uint64_t np = a253889_nextp(p);
            while (r % p == 0) {
                r /= p;
                prod = prod * (seq_t)np;
            }
        }
    }
    if (r > 1) prod = prod * (seq_t)a253889_nextp(r);

    return (prod + (seq_t)1) / (seq_t)2;
}
