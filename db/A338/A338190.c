/*
 * A338190  Integers m such that A337454(m) = tau(m), the number of divisors
 *          of n.
 * https://oeis.org/A338190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m such that A337454(m) = tau(m); found by
 *            search, with the counts in A337454 computed multiplicatively
 *            over the prime powers of m (CRT)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A338190_pm(uint64_t b, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    b %= mod;
    while (e) {
        if (e & 1) r = (uint64_t)(((unsigned __int128)r * b) % mod);
        b = (uint64_t)(((unsigned __int128)b * b) % mod);
        e >>= 1;
    }
    return r;
}

static int A338190_is_term(uint64_t m)
{
    uint64_t q[20];
    int k = 0;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t pe = 1;
            while (t % p == 0) { t /= p; pe *= p; }
            q[k++] = pe;
        }
    }
    if (t > 1) q[k++] = t;

    uint64_t tau = 0, good = 0;
    for (uint64_t d = 1; d <= m; ++d) {
        if (m % d) continue;
        ++tau;
        uint64_t c1 = 1, c2 = 1;
        for (int i = 0; i < k; ++i) {
            uint64_t n1 = 0, n2 = 0, mod = q[i];
            for (uint64_t x = 0; x < mod; ++x) {
                uint64_t p = A338190_pm(x, d, mod);
                if (p == x) ++n1;
                if ((p + x) % mod == 0) ++n2;
            }
            c1 *= n1;
            c2 *= n2;
        }
        if (c2 != 0 && c1 % c2 == 0) {
            uint64_t ratio = c1 / c2;
            if (m % ratio == 0) ++good;
        }
    }
    return good == tau;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static uint64_t cnt = 0, cap = 0, next_m = 1;
    while (cnt < (uint64_t)n) {
        if (A338190_is_term(next_m)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 256;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = next_m;
        }
        ++next_m;
    }
    return (seq_t)terms[n - 1];
}
