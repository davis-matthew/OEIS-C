/*
 * A323129  a(1) = 1, and for any n > 1, let p be the greatest prime factor of
 *          n, and e be its exponent, then a(n) = p^a(e).
 * https://oeis.org/A323129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   a(n) = 1 if n = 1; otherwise if p is the greatest prime factor
 *            of n and e its exponent in n, then a(n) = p^a(e)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, ensure_cache, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gpf.h"
#include "../utils/big_pow.h"
#include "../utils/ensure_cache.h"

static seq_t *memo = NULL;      /* cache for computed a(n) values */
static size_t memo_cap = 0;     /* current capacity of memo */

/* Recursive helper with memoization */
static seq_t _a_rec(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (memo && n <= memo_cap && memo[n - 1] != (seq_t)0)
        return memo[n - 1];

    uint64_t p = gpf(n);          /* greatest prime factor of n */
    idx_t    e = 0;
    uint64_t t = n;
    while (t % p == 0) {
        ++e;
        t /= p;
    }

    seq_t a_e = _a_rec(e);           /* a(e) */
    seq_t val = big_pow((seq_t)p, a_e); /* p^a(e) */

    ensure_cache(n, &memo, &memo_cap); /* make sure memo has room for n */
    memo[n - 1] = val;                /* store the computed value */

    return val;
}

/* Public interface required by the build system */
seq_t a(idx_t n)
{
    return _a_rec(n);
}
