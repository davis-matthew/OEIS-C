/*
 * A391734  Right edge of triangle A240673: a(n) = A002110(n-1) * A079276(n).
 * https://oeis.org/A391734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002110(n-1) * A079276(n) = (prime(1)*...*prime(n-1)) *
 *            ((prime(1)*...*prime(n-1))^(-1) mod prime(n)), a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 327 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t rt_pow_mod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (r * b) % m;
        b = (b * b) % m;
        e >>= 1;
    }
    return r;
}

static bool rt_is_prime(uint64_t x)
{
    uint64_t d;
    if (x < 2) return false;
    for (d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    seq_t prim = 1;          /* product of first n-1 primes */
    uint64_t cnt = 0, c = 1;
    uint64_t pn;
    uint64_t *ps;
    uint64_t i, res, inv;

    if (n < 1) return 0;
    ps = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!ps) abort();
    while (cnt < (uint64_t)n) {
        c++;
        if (rt_is_prime(c)) ps[cnt++] = c;
    }
    pn = ps[n - 1];
    res = 1 % pn;
    for (i = 0; i + 1 < (uint64_t)n; i++) {
        prim *= (seq_t)ps[i];
        res = (res * (ps[i] % pn)) % pn;
    }
    free(ps);
    inv = rt_pow_mod(res, pn - 2, pn);
    return prim * (seq_t)inv;
}
