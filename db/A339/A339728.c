/*
 * A339728  Odd composite integers m such that A005248(3*m-J(m,5)) == 7 (mod
 *          m), where J(m,5) is the Jacobi symbol.
 * https://oeis.org/A339728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd composite integer m such that V(3*m - J(m,5))
 *            ≡ 7 (mod m), where V is the Lucas sequence V_0=2, V_1=3, V_{k} =
 *            3*V_{k-1} - V_{k-2}
 * Range:     u8 n <= 7, u16 n <= 160, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol, mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/jacobi_symbol.h"
#include "../utils/mul64.h"

/* Compute V(n) modulo m for the Lucas sequence with a = 3, b = 1. */
static seq_t V_mod(uint64_t m, uint64_t n)
{
    if (n == 0) return (seq_t)(2 % m);
    if (n == 1) return (seq_t)(3 % m);

    /* 2x2 matrix for the recurrence: [[3, -1], [1, 0]] */
    struct Mat { uint64_t a00, a01, a10, a11; };

    struct Mat res = { 1 % m, 0, 0, 1 % m };          /* identity */
    struct Mat base = { 3 % m, (m - 1) % m, 1 % m, 0 }; /* [[3,-1],[1,0]] mod m */

    uint64_t e = n;
    while (e) {
        if (e & 1) {
            struct Mat t;
            t.a00 = (mul64(res.a00, base.a00, m) + mul64(res.a01, base.a10, m)) % m;
            t.a01 = (mul64(res.a00, base.a01, m) + mul64(res.a01, base.a11, m)) % m;
            t.a10 = (mul64(res.a10, base.a00, m) + mul64(res.a11, base.a10, m)) % m;
            t.a11 = (mul64(res.a10, base.a01, m) + mul64(res.a11, base.a11, m)) % m;
            res = t;
        }
        struct Mat t;
        t.a00 = (mul64(base.a00, base.a00, m) + mul64(base.a01, base.a10, m)) % m;
        t.a01 = (mul64(base.a00, base.a01, m) + mul64(base.a01, base.a11, m)) % m;
        t.a10 = (mul64(base.a10, base.a00, m) + mul64(base.a11, base.a10, m)) % m;
        t.a11 = (mul64(base.a10, base.a01, m) + mul64(base.a11, base.a11, m)) % m;
        base = t;
        e >>= 1;
    }

    /* V(n) = res*a1 + res*a0  with a1=3, a0=2 */
    uint64_t v1 = (mul64(res.a10, 3 % m, m) + mul64(res.a11, 2 % m, m)) % m;
    return (seq_t)v1;
}

/* Cache of discovered sequence terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_checked = 7;    /* last odd value examined */

/* Append a new term to the cache, growing storage as needed. */
static void cache_append(seq_t val)
{
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Generate terms until we have at least n values. */
static void extend_cache(idx_t n)
{
    while ((idx_t)cache_len < n) {
        /* Advance to next odd candidate. */
        if (last_checked < 9) last_checked = 9;
        else last_checked += 2;

        /* Skip primes. */
        if (is_prime(last_checked)) continue;

        /* Jacobi symbol J(last_checked,5). */
        int j = jacobi_symbol(last_checked, 5);

        /* Compute exponent e = 3*last_checked - J. */
        uint64_t e = last_checked * 3ull;
        if (j == 1)      e -= 1;
        else if (j == -1) e += 1;   /* j==0 leaves e unchanged */

        /* Evaluate V(e) mod last_checked. */
        seq_t v = V_mod(last_checked, e);

        /* Check if condition holds. */
        if (v == (seq_t)(7 % last_checked))
            cache_append((seq_t)last_checked);
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* behavior below offset is irrelevant */
    extend_cache(n);
    return cache[n - 1];
}
