/*
 * A251182  a(n) = Sum_{k=0..n} binomial(n, k) * (2^k - 1)^k.
 * https://oeis.org/A251182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (2^k - 1)^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-03)
 * Helpers:   ipow_i32, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"
#include "../utils/mem.h"

/* Cache for previously computed values of a(n). */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t computed_max = 0;
static bool cache_inited = false;

/* Cache for (2^k - 1)^k. */
static seq_t *t_cache = NULL;
static idx_t t_cap = 0;

/* Ensure cache array can hold index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    if (!new_cache)
        abort();
    for (idx_t i = cache_cap; i <= n; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

/* Ensure t_cache array can hold index n. */
static void ensure_t_cache(idx_t n)
{
    if (n < t_cap)
        return;
    size_t new_cap = (size_t)n + 1;
    seq_t *new_t = mem_resize(seq_t, t_cache, new_cap);
    if (!new_t)
        abort();
    for (idx_t i = t_cap; i <= n; ++i)
    {
        if (i == 0)
        {
            new_t[i] = 1;                       /* (2^0-1)^0 = 1 */
        }
        else
        {
            seq_t base   = pow2(i);             /* 2^i */
            seq_t base_minus_one = base - 1;    /* 2^i - 1 */
            new_t[i] = ipow_i32(base_minus_one, (int)i);
        }
    }
    t_cache = new_t;
    t_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    ensure_t_cache(n);

    if (!cache_inited)
    {
        cache[0] = 1;
        cache_inited = true;
        computed_max = 0;
    }

    if (n <= computed_max)
        return cache[n];

    for (idx_t i = computed_max + 1; i <= n; ++i)
    {
        seq_t sum = 0;
        seq_t bin = 1; /* binomial(i,0) */
        for (idx_t k = 0; k <= i; ++k)
        {
            seq_t term = bin * t_cache[k];
            sum += term;
            if (k < i)
                bin = bin * (seq_t)(i - k) / (k + 1);
        }
        cache[i] = sum;
    }
    computed_max = n;
    return cache[n];
}
