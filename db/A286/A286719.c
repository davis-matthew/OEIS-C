/*
 * A286719  Column k=4 of triangle A039755, Sheffer(exp(x), (exp(2*x) - 1)/2).
 * https://oeis.org/A286719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A286719
   Formula (closed form):  a(n) = (1 - 4*3^(4+n) + 6*5^(4+n) - 4*7^(4+n) + 9^(4+n)) / 384
   Recurrence (order 5): a(n) = 25*a(n-1) - 230*a(n-2) + 950*a(n-3) - 1689*a(n-4) + 945*a(n-5)
   Initial values: a(0)=1, a(1)=25, a(2)=395, a(3)=5075, a(4)=58086
*/

#include <stdlib.h>

static seq_t *cache = NULL;    /* dynamic cache for computed terms */
static idx_t cache_len = 0;    /* number of elements in cache */

/* initialise cache with the first 5 terms */
static void init_cache(void)
{
    cache_len = 5;
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
    cache[0] = (seq_t)1;      /* a(0) */
    cache[1] = (seq_t)25;     /* a(1) */
    cache[2] = (seq_t)395;    /* a(2) */
    cache[3] = (seq_t)5075;   /* a(3) */
    cache[4] = (seq_t)58086;  /* a(4) */
}

/* grow the cache to at least new_len elements */
static void grow_cache(idx_t new_len)
{
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!new_cache) abort();   /* allocation failed */
    cache = new_cache;
    cache_len = new_len;
}

/* compute all terms up to index n (inclusive) */
static void compute_up_to(idx_t n)
{
    if (cache_len == 0) init_cache();

    if (n < cache_len) return;           /* nothing to do */

    idx_t old_len = cache_len;            /* first new index to compute */
    grow_cache(n + 1);

    for (idx_t i = old_len; i <= n; ++i) {
        /* recurrence: a(i) = 25*a(i-1) - 230*a(i-2) + 950*a(i-3) - 1689*a(i-4) + 945*a(i-5) */
        seq_t val = 25 * cache[i-1]
                  - 230 * cache[i-2]
                  + 950 * cache[i-3]
                  - 1689 * cache[i-4]
                  + 945 * cache[i-5];
        cache[i] = val;
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return cache[n];
}
