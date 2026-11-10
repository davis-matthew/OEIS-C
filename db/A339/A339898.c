/*
 * A339898  a(n) = A019565(2n)-1 mod A000265(phi(A019565(2n))).
 * https://oeis.org/A339898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A019565(2n) - 1) mod A000265(phi(A019565(2n)));
 *            A019565(2n) is squarefree, so phi = Product (p-1) over its
 *            primes
 * Range:     u8 n <= 13, u16 n <= 61, u32 n <= 3945, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 110 terms through n = 7962 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint32_t A339898_prime(int k) /* k-th prime, k >= 1, by trial division */
{
    uint32_t cnt = 0, c = 1;
    while (true) {
        ++c;
        bool ok = true;
        for (uint32_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = false; break; }
        }
        if (ok && ++cnt == (uint32_t)k) return c;
    }
}

seq_t a(idx_t n)
{
    static uint32_t pr[70];
    static bool init = false;
    if (!init) {
        for (int i = 0; i < 70; ++i) pr[i] = A339898_prime(i + 1);
        init = true;
    }
    seq_t m = 1;
    seq_t phi = 1;
    int bit = 0;
    idx_t t = n;
    while (t != 0) {
        if (t & 1) {
            /* bit `bit` of n is bit bit+1 of 2n -> prime index bit+2 (1-based) */
            uint32_t p = pr[bit + 1];
            m *= (seq_t)p;
            phi *= (seq_t)(p - 1);
        }
        t >>= 1;
        ++bit;
    }
    while ((phi & 1) == 0) phi >>= 1;
    return (m - 1) % phi;
}
