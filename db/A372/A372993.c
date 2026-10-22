/*
 * A372993  a(n) = n! a(n-1) / a(n-2), where a(0) = 1, a(1) = 1.
 * https://oeis.org/A372993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n! * a(n-1) / a(n-2),  a(0)=1, a(1)=1 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__SIZEOF_INT128__)
#  define HAVE_INT128 1
#else
#  define HAVE_INT128 0
#endif

static seq_t *cache      = NULL;  /* memoised a(i) values   */
static size_t  cache_cap = 0;     /* current allocation size */
static size_t  cache_len = 0;     /* number of values computed  */
static seq_t   fact       = 1;    /* factorial of fact_n */
static idx_t   fact_n     = 0;    /* index up to which factorial is computed */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* trivial base values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* ensure cache capacity (grow to the next power of two ≥ n+1) */
    if (n + 1 > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        while (newcap <= n + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cache_cap = newcap;
    }

    /* initialise base terms in cache if empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len < 2 && n >= 1) {
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* already computed? */
    if (n < cache_len) return cache[n];

    /* compute successive terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* extend factorial to i! */
        while (fact_n < i) {
            ++fact_n;
            fact = fact * (seq_t)fact_n;   /* may overflow; acceptable for non‑fit tests */
        }
        if (i < 2) continue;   /* base terms already set */

        /* compute a(i) = a(i-1) * i! / a(i-2) */
#if HAVE_INT128 && defined(__SIZEOF_INT128__)
        /* use 128‑bit intermediate to avoid overflow on small built‑in types */
        if (sizeof(seq_t) <= 8) {
            unsigned __int128 prod = (unsigned __int128)fact * (unsigned __int128)cache[i-1];
            cache[i] = (seq_t)(prod / (unsigned __int128)cache[i-2]);
        } else
#endif
        {
            /* generic case: rely on built‑in operator overloading or integral arithmetic */
            cache[i] = cache[i-1] * fact / cache[i-2];
        }
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
