/*
 * A255371  Number of strings of n decimal digits that contain at least one
 *          "0" digit that is not part of a string of two or more consecutive
 *          "0" digits.
 * https://oeis.org/A255371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 0 for n=0 a(1) = 1 for n>=2: a(n) = 9 * (10^(n-2) -
 *            a(n-2) + Σ_{i=1}^{n-1} a(i)) Recurrence used: a(0)=0, a(1)=1,
 *            a(n)=9*(10^(n-2)-a(n-2)+sum_{i=1}^{n-1}a(i)) (same formula as in
 *            the OEIS entry)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow10.h"

static seq_t *cache   = NULL;   /* cached a(n) values   */
static seq_t *sumd    = NULL;   /* prefix sums Σ_{i=0}^n a(i) */
static size_t cache_cap = 0;    /* allocated capacity    */
static size_t last_computed = 0;/* highest index computed */
static int   initialized = 0;   /* cache initialized flag */

/* allocate or grow the cache arrays up to size n+1 */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap) return;

    size_t newcap = cache_cap ? cache_cap * 2 : 8;
    while (newcap < needed) newcap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    seq_t *new_sumd  = (seq_t *)realloc(sumd,  newcap * sizeof(seq_t));
    if (!new_cache || !new_sumd) exit(EXIT_FAILURE);

    cache = new_cache;
    sumd  = new_sumd;
    cache_cap = newcap;
}

/* initialise the base cases a(0)=0 and a(1)=1 */
static void init_cache(void)
{
    if (initialized) return;
    ensure_cache(1);            /* room for indices 0 and 1 */

    cache[0] = (seq_t)0;
    sumd[0]  = (seq_t)0;
    cache[1] = (seq_t)1;
    sumd[1]  = sumd[0] + cache[1];

    last_computed = 1;
    initialized = 1;
}

seq_t a(idx_t n)
{
    init_cache();                      /* ensure base cases are set */

    /* If the value is already computed, just return it */
    if ((size_t)n <= last_computed) return cache[(size_t)n];

    /* Extend the cache up to n */
    ensure_cache(n);

    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        seq_t pow   = pow10((idx_t)(i-2));          /* 10^(i-2)    */
        seq_t diff  = pow - cache[i-2];             /* 10^(i-2)-a(i-2) */
        seq_t sum_prev = sumd[i-1];                 /* Σ_{k=0}^{i-1} a(k) */
        seq_t val = (seq_t)9 * (diff + sum_prev);  /* a(i)         */
        cache[i] = val;
        sumd[i]  = sum_prev + val;
    }

    last_computed = (size_t)n;
    return cache[(size_t)n];
}
