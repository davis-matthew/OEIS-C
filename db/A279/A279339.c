/*
 * A279339  a(1) = 1; for n > 1, if n is even, a(n) = A055938(a(n/2)),
 *          otherwise a(n) = A005187(a(A064989(n))).
 * https://oeis.org/A279339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = A055938(a(n/2)) if n is even,
 *            otherwise a(n) = A005187(a(A064989(n)))
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 138, u64 n <= 141,
 *            u128 n <= 141, big any n
 * Validated: standard, pass, 94 terms through n = 136 (2026-11-08)
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"

static unsigned a279339_pop(seq_t k)
{
    unsigned c = 0;
    while (k > 0) {
        if ((k & 1) != 0) c++;
        k >>= 1;
    }
    return c;
}

/* A005187(k) = k + floor(k/2) + floor(k/4) + ... */
static seq_t A005187_a(seq_t m)
{
    seq_t result = 0;
    seq_t p = m;
    while (p > 0) {
        result += p;
        p >>= 1;
    }
    return result;
}

/* number of members of A005187 (values 2k - popcount(k), increasing) that are <= x */
static seq_t a279339_count_members(seq_t x)
{
    seq_t lo = 0;
    seq_t hi = x / 2 + 64;
    /* largest k with k <= (x + popcount(k)) / 2 ; k = 0 always qualifies */
    while (lo < hi) {
        seq_t mid = lo + (hi - lo + 1) / 2;
        if (mid <= (x + (seq_t)a279339_pop(mid)) / 2)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo + 1;
}

/* A055938(m): m-th positive integer not of the form A005187(k) */
static seq_t A055938_a(seq_t m)
{
    seq_t x = 0;
    for (;;) {
        seq_t nx = m - 1 + a279339_count_members(x);
        if (nx == x) break;
        x = nx;
    }
    return x;
}

static idx_t A064989_prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p) {
        if (is_prime(p)) return p;
    }
}

static idx_t A064989_a(idx_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    idx_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        idx_t base = A064989_prev_prime((idx_t)pr[i]);
        for (uint64_t e = 0; e < ex[i]; ++e)
            result *= base;
    }
    return result;
}

static seq_t a279339_rec(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if ((n & 1) == 0)
        return A055938_a(a279339_rec(n / 2));
    return A005187_a(a279339_rec(A064989_a(n)));
}

seq_t a(idx_t n)
{
    return a279339_rec(n);
}
