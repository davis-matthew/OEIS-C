/*
 * A258368  Smallest odd prime p such that A258367(p) = n, or 0 if no such p
 *          exists.
 * https://oeis.org/A258368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = smallest odd prime p with min(A, p-A) = n where
 *            2^((p-1)/2) == +-1 + A*p (mod p^2); computed by incremental
 *            search over primes
 * Range:     u8 none, u16 n <= 35, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 43 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 A258368_u128;

#define A258368_K 64

static uint64_t A258368_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = (uint64_t)((A258368_u128)r * b % m);
        b = (uint64_t)((A258368_u128)b * b % m);
        e >>= 1;
    }
    return r;
}

static bool A258368_isprime(uint64_t p)
{
    uint64_t d;
    if (p < 2) return false;
    if (p % 2 == 0) return p == 2;
    for (d = 3; d * d <= p; d += 2)
        if (p % d == 0) return false;
    return true;
}

static uint64_t A258368_val(uint64_t p)
{
    uint64_t m = p * p;
    uint64_t S = A258368_powmod(2, (p - 1) / 2, m);
    uint64_t A;
    if (S % p == 1) A = (S - 1) / p;
    else A = (S + 1) / p;
    if (A > p) return p;
    return (A <= p - A) ? A : p - A;
}

seq_t a(idx_t n)
{
    static uint64_t first[A258368_K];
    static uint64_t cand = 3;
    const uint64_t limit = 4294967291ULL;
    if (n >= A258368_K) return 0;
    while (first[n] == 0 && cand < limit) {
        uint64_t p = cand;
        cand += 2;
        if (!A258368_isprime(p)) continue;
        {
            uint64_t v = A258368_val(p);
            if (v < A258368_K && first[v] == 0) first[v] = p;
        }
    }
    return (seq_t)first[n];
}
