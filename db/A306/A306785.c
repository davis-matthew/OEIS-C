/*
 * A306785  Primes p such that p^2 divides A050443(p).
 * https://oeis.org/A306785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, bref
 * Formula:   a(n) = n-th prime p with A050443(p) == 0 (mod p^2), where
 *            A050443(0)=4, A050443(1)=0, A050443(2)=0, A050443(3)=3,
 *            A050443(n)=A050443(n-3)+A050443(n-4); computed by search over
 *            primes, using x^p mod (x^4-x-1) modulo p^2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 A306785_u128;

static void A306785_mul(const A306785_u128 *x, const A306785_u128 *y, A306785_u128 *r, A306785_u128 m)
{
    A306785_u128 d[7];
    int i, j;
    for (i = 0; i < 7; i++) d[i] = 0;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 4; j++)
            d[i + j] = (d[i + j] + x[i] * y[j]) % m;
    r[0] = (d[0] + d[4]) % m;
    r[1] = (d[1] + d[4] + d[5]) % m;
    r[2] = (d[2] + d[5] + d[6]) % m;
    r[3] = (d[3] + d[6]) % m;
}

static bool A306785_check(uint64_t p)
{
    A306785_u128 m = (A306785_u128)p * p;
    A306785_u128 res[4] = {1, 0, 0, 0};
    A306785_u128 base[4] = {0, 1, 0, 0};
    uint64_t e = p;
    while (e) {
        if (e & 1) A306785_mul(res, base, res, m);
        A306785_mul(base, base, base, m);
        e >>= 1;
    }
    A306785_u128 s = (4 * res[0] + 3 * res[3]) % m;
    return s == 0;
}

static bool A306785_isprime(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t found[64];
    static int cnt = 0;
    static uint64_t next = 2;
    while ((idx_t)cnt < n && cnt < 64 && next < 4000000ULL) {
        uint64_t p = next++;
        if (A306785_isprime(p) && A306785_check(p))
            found[cnt++] = p;
    }
    if (n >= 1 && (idx_t)cnt >= n) return (seq_t)found[n - 1];
    return (seq_t)0;
}
