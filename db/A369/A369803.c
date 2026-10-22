/*
 * A369803  Expansion of 1/(1 - x^2/(1-x)^5).
 * https://oeis.org/A369803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n-1+3*k , n-2*k) Recurrence:
 *            a(n) = 5*a(n-1) – 9*a(n-2) + 10*a(n-3) – 5*a(n-4) + a(n-5) for n
 *            > 5 Initial values: a(0)=1 , a(1)=0 , a(2)=1 , a(3)=5 , a(4)=16
 *            , a(5)=45
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


static seq_t *cache = NULL;   /* array of computed terms          */
static size_t cache_len = 0;  /* number of terms stored (index+1) */
static size_t cache_cap = 0;  /* allocated capacity                */

static void init_cache(void)
{
    if (cache != NULL)
        return;
    cache_cap = 64;                     /* start with a reasonable size*/
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (cache == NULL)
        exit(1);                       /* unlikely in test harness     */
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)5;
    cache[4] = (seq_t)16;
    cache[5] = (seq_t)45;
    cache_len = 6;                    /* we have values for indices 0..5 */
}

static void grow_cache(idx_t needed)
{
    if (needed < cache_len)
        return;

    /* enlarge capacity if necessary */
    while (cache_cap <= needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            exit(1);                   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms from cache_len to needed */
    for (idx_t i = cache_len; i <= needed; ++i) {
        /* recurrence is valid for i > 5 */
        if (i <= 5)
            continue;
        seq_t t1 = (seq_t)5 * cache[i - 1];
        seq_t t2 = (seq_t)9 * cache[i - 2];
        seq_t t3 = (seq_t)10 * cache[i - 3];
        seq_t t4 = (seq_t)5 * cache[i - 4];
        seq_t res = t1 - t2;
        res = res + t3;
        res = res - t4;
        res = res + cache[i - 5];
        cache[i] = res;
    }

    cache_len = needed + 1;   /* terms up to 'needed' are now stored */
}

seq_t a(idx_t n)
{
    init_cache();
    if (n < cache_len)
        return cache[n];
    grow_cache(n);
    return cache[n];
}
