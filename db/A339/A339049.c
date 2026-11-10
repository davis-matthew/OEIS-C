/*
 * A339049  a(n) = A000010(2*n + 1)/A053447(n), for n >= 0.
 * https://oeis.org/A339049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = phi(2*n+1)/A053447(n), where A053447(n) is the
 *            multiplicative order of 4 mod 2n+1 (a(0)=1)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 78 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A339049_mulmod(uint64_t x, uint64_t y, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)x * y) % m);
}

static uint64_t A339049_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A339049_mulmod(r, b, m);
        b = A339049_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    uint64_t M = (uint64_t)n * 2 + 1;
    if (M == 1) return (seq_t)1;

    /* phi(M) */
    uint64_t phi = M, m = M, p;
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (m > 1) phi = phi / m * (m - 1);

    /* order of 4 mod M: reduce phi by its prime factors */
    uint64_t order = phi, t = phi;
    uint64_t q;
    for (q = 2; q <= t / q; q++) {
        if (t % q == 0) {
            while (t % q == 0) t /= q;
            while (order % q == 0 && A339049_powmod(4, order / q, M) == 1)
                order /= q;
        }
    }
    if (t > 1) {
        while (order % t == 0 && A339049_powmod(4, order / t, M) == 1)
            order /= t;
    }

    return (seq_t)(phi / order);
}
