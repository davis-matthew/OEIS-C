/*
 * A368811  a(n) = period length of the sequence A020639(n^k - 1), k >= 1.
 * https://oeis.org/A368811
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm of ord_p(n) over primes p < lpf(n-1), p not dividing
 *            n, whose order is not a multiple of a smaller such prime's order
 *            (smallest prime factor of n^k-1 is the least p with ord_p(n) |
 *            k); a(n)=1 for odd n. Computed by search
 * Range:     u8 n <= 29, u16 n <= 59, u32 n <= 109, u64 n <= 197,
 *            u128 n <= 419, big any n
 * Validated: standard, pass, 116 terms through n = 9971 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool a368811_is_prime(uint64_t m)
{
    if (m < 2) return false;
    if (m % 2 == 0) return m == 2;
    for (uint64_t i = 3; i <= m / i; i += 2)
        if (m % i == 0) return false;
    return true;
}

static seq_t a368811_gcd(seq_t x, seq_t y)
{
    while (y != 0) {
        seq_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)1;
    if ((n & 1) != 0) return (seq_t)1;

    /* least prime factor q of n-1 (odd, >= 3) */
    uint64_t m = (uint64_t)n - 1;
    uint64_t q = m;
    for (uint64_t i = 3; i <= m / i; i += 2) {
        if (m % i == 0) { q = i; break; }
    }

    size_t cap = 16, cnt = 0;
    uint64_t *ds = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!ds) return (seq_t)0;
    seq_t L = (seq_t)1;

    for (uint64_t p = 3; p < q; p += 2) {
        if (!a368811_is_prime(p)) continue;
        uint64_t r = (uint64_t)n % p;
        if (r == 0) continue;
        /* multiplicative order of r mod p */
        uint64_t d = 1, x = r;
        while (x != 1) {
            x = (x * r) % p;
            d++;
        }
        bool eff = true;
        for (size_t i = 0; i < cnt; i++) {
            if (d % ds[i] == 0) { eff = false; break; }
        }
        if (!eff) continue;
        if (cnt == cap) {
            cap *= 2;
            uint64_t *t = (uint64_t *)realloc(ds, cap * sizeof(uint64_t));
            if (!t) { free(ds); return (seq_t)0; }
            ds = t;
        }
        ds[cnt++] = d;
        seq_t sd = (seq_t)d;
        L = L / a368811_gcd(L, sd) * sd;
    }
    free(ds);
    return L;
}
