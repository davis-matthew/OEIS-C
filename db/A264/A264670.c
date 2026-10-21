/*
 * A264670  Number of (n+1) X (2+1) arrays of permutations of 0..n*3+2 with
 *          each element having directed index change -1,-1 1,0 -1,-2 -2,-2 or
 *          0,1.
 * https://oeis.org/A264670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-3) + 3*a(n-4) + 2*a(n-5) - a(n-6) +
 *            4*a(n-7) - a(n-8) - a(n-10) for n ≥ 11; a(1)=1, a(2)=2, a(3)=9,
 *            a(4)=19, a(5)=44, a(6)=108, a(7)=264, a(8)=649, a(9)=1573,
 *            a(10)=3837
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stores a[1]..a[cache_len] */
static idx_t cache_len = 0;          /* number of elements stored */

/* Initial values for a(1) .. a(10) */
static const seq_t init_vals[10] = {
    (seq_t)1,  (seq_t)2,  (seq_t)9,  (seq_t)19, (seq_t)44,
    (seq_t)108,(seq_t)264,(seq_t)649,(seq_t)1573,(seq_t)3837
};

static void init_cache(void)
{
    if (cache != NULL)
        return;
    /* Start with space for at least 10 elements */
    idx_t need = 10;
    cache = (seq_t *)malloc(need * sizeof(seq_t));
    for (idx_t i = 0; i < 10; ++i)
        cache[i] = init_vals[i];
    cache_len = 10;
}

/* Compute a(n) for n ≥ 1 using the recurrence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined; guard against underflow */
    init_cache();
    if (n <= cache_len)
        return cache[n-1];

    /* Grow cache up to n */
    idx_t new_len = n;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* Compute terms from cache_len+1 to n */
    for (idx_t k = cache_len + 1; k <= n; ++k) {
        /* recurrence: a(k) = 2*a(k-1) + a(k-3) + 3*a(k-4) + 2*a(k-5)
                         - a(k-6) + 4*a(k-7) - a(k-8) - a(k-10) */
        seq_t term = 0;

        term += (seq_t)2 * cache[k-2];   /* 2*a(k-1) */
        term += cache[k-4];              /* a(k-3)     */
        term += (seq_t)3 * cache[k-5];   /* 3*a(k-4)   */
        term += (seq_t)2 * cache[k-6];   /* 2*a(k-5)   */
        term -= cache[k-7];              /* -a(k-6)    */
        term += (seq_t)4 * cache[k-8];   /* 4*a(k-7)   */
        term -= cache[k-9];              /* -a(k-8)    */
        term -= cache[k-11];             /* -a(k-10)   */

        cache[k-1] = term;
    }
    cache_len = n;
    return cache[n-1];
}
