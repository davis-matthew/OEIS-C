/*
 * A351466  a(n) = (a(n-1) + a(n-3))/2 if (a(n-1)+a(n-3)) is even. Otherwise,
 *          a(n)=a(n-1)+a(n-3).
 * https://oeis.org/A351466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 75, u32 n <= 139, u64 n <= 278,
 *            u128 n <= 549, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351466:
   a(1)=1, a(2)=2, a(3)=1
   a(n) = (a(n-1)+a(n-3))/2   if (a(n-1)+a(n-3)) is even
           a(n-1)+a(n-3)       otherwise
   (offset 1) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* array holding cached terms */
static idx_t  cache_len   = 0;       /* currently allocated length */
static idx_t  max_computed = 0;      /* largest n whose value is stored */
static bool   initialized = false;   /* whether first three terms are set */

/* Ensure that the cache can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n)
        return;

    idx_t new_len = n + 1;                /* need space for indices 0..n */
    if (new_len < 4)
        new_len = 4;                      /* small initial size */
    if (new_len < cache_len * 2)
        new_len = cache_len * 2;          /* exponential growth */

    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort. */
        exit(EXIT_FAILURE);
    }
    cache     = tmp;
    cache_len = new_len;
}

/* Compute the next term a(n)=f(a(n-1),a(n-3)) safely without overflow. */
#ifdef __cplusplus
#include <type_traits>
static inline seq_t next_term(seq_t a1, seq_t a3)
{
    if constexpr (std::is_integral_v<seq_t>) {
        using uint128 = unsigned __int128;
        uint128 s = (uint128)a1 + (uint128)a3;
        if ((s & 1) == 0)
            return (seq_t)(s >> 1);
        else
            return (seq_t)s;
    } else {
        /* Non‑integral (big‑int) build */
        auto s = a1 + a3;
        if ((s & 1) == 0)
            return s / 2;
        else
            return s;
    }
}
#else
static inline seq_t next_term(seq_t a1, seq_t a3)
{
    /* Use 128‑bit arithmetic to avoid overflow on integral seq_t types. */
    __uint128_t s = (__uint128_t)a1 + (__uint128_t)a3;
    if ((s & 1) == 0)
        return (seq_t)(s >> 1);
    else
        return (seq_t)s;
}
#endif

/* Initialise the first three (known) terms of the sequence. */
static void init_cache(void)
{
    if (initialized)
        return;

    ensure_cache(3);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)1;
    initialized  = true;
    max_computed = 3;
}

/* Return a(n) for any n ≥ 1 (a(0) is undefined – return 0). */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;                 /* undefined, but safe */

    init_cache();                       /* ensure base terms exist */

    /* If the requested term is already cached, simply return it. */
    if (n <= max_computed)
        return cache[n];

    /* Grow the cache to accommodate up to n. */
    ensure_cache(n);

    /* Compute missing terms iteratively. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = next_term(cache[i - 1], cache[i - 3]);
    }
    max_computed = n;
    return cache[n];
}
