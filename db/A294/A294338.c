/*
 * A294338  Number of ways to write n as a finite power-tower of positive
 *          integers greater than one, allowing both left and right nesting of
 *          parentheses.
 * https://oeis.org/A294338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1;
   a(n) = 1 + Σ_{a^b = n, a>1, b>1} a(a) · a(b)          (OEIS A294338) */
#include <stdlib.h>
#include <stdint.h>

/* cache for already computed values, 1‑indexed */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* enlarge cache up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    size_t sz = (size_t)(n + 1);                /* need indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, sz * sizeof(seq_t));
    if (!tmp)
        abort();                               /* allocation failed */
    cache = tmp;

    /* initialise new entries to 0 (except the first useful one) */
    for (idx_t i = cache_size; i <= n; ++i)
        cache[i] = (seq_t)0;

    /* initialise base case a(1)=1 */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;                   /* unused */
        cache[1] = (seq_t)1;
    }

    cache_size = sz;
}

/* integer square‑root with 64‑bit arithmetic (works for n ≤ 2^64‑1) */
static idx_t isqrt(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

/* recursive computation with memoisation */
static seq_t rec(idx_t n)
{
    ensure_cache(n);

    if (cache[n] != (seq_t)0)            /* already computed */
        return cache[n];

    /* trivial representation (the number itself) */
    seq_t res = 1;

    /* search for all decompositions a^b = n (with a,b>1) */
    idx_t limit = isqrt(n);               /* a <= sqrt(n) */
    for (idx_t a = 2; a <= limit; ++a) {
        if (n % a != 0)
            continue;                     /* a must divide n */

        idx_t exp = 2;
        idx_t prod = (idx_t)(a * a);
        while (prod < n) {
            if (prod > n / a)              /* would overflow n */
                break;
            prod *= a;
            ++exp;
        }

        if (prod == n) {                   /* a^exp == n discovered */
            res += rec(a) * rec(exp);
        }
    }

    cache[n] = res;
    return res;
}

/* the required exports */
seq_t a(idx_t n)
{
    if (n == 0)                            /* undefined in OEIS */
        return (seq_t)0;
    return rec(n);
}
