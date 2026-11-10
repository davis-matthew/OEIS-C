/*
 * A328617  Multiplicative with a(p^e) = p^e, if e = 0 mod p, otherwise a(p^e)
 *          = p^((p*floor(e/p)) + A124223(A000720(p),e mod p)).
 * https://oeis.org/A328617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} p^E, E = e if e mod p = 0, else
 *            p*floor(e/p) + (inverse of (e mod p) modulo p); multiplicative
 * Range:     u8 n <= 48, u16 n <= 120, u32 n <= 288, u64 n <= 840,
 *            u128 n <= 2208, big any n
 * Validated: standard, pass, 97 terms through n = 9139 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t A328617_inv(uint64_t r, uint64_t p)
{
    /* modular inverse of r mod prime p via extended Euclid */
    long long t0 = 0, t1 = 1;
    long long a0 = (long long)p, a1 = (long long)(r % p);
    while (a1 != 0) {
        long long q = a0 / a1;
        long long tmp = a0 - q * a1; a0 = a1; a1 = tmp;
        tmp = t0 - q * t1; t0 = t1; t1 = tmp;
    }
    if (t0 < 0) t0 += (long long)p;
    return (uint64_t)t0;
}

static seq_t A328617_term(uint64_t p, uint64_t e)
{
    uint64_t r = e % p;
    uint64_t ex;
    if (r == 0) ex = e;
    else ex = p * (e / p) + A328617_inv(r, p);
    seq_t res = 1;
    for (uint64_t i = 0; i < ex; ++i) res *= (seq_t)p;
    return res;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    seq_t result = 1;
    if (m <= 1) return 1;
    for (uint64_t d = 2; d * d <= m; ++d) {
        if (m % d == 0) {
            uint64_t e = 0;
            while (m % d == 0) { m /= d; ++e; }
            result *= A328617_term(d, e);
        }
    }
    if (m > 1) result *= A328617_term(m, 1);
    return result;
}
