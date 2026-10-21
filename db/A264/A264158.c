/*
 * A264158  Number of (n+1) X (1+1) arrays of permutations of 0..n*2+1 with
 *          each element having directed index change 0,1 0,-1 0,2 1,0 -1,0 or
 *          2,0.
 * https://oeis.org/A264158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) + 4*a(n-5) - a(n-8) *
 *            Initial values: a(1)=4, a(2)=10, a(3)=29, a(4)=89, * a(5)=260,
 *            a(6)=772, a(7)=2281, a(8)=6741. * * This implementation uses a
 *            simple memoised recurrence. The cache * is grown with realloc so
 *            that previously‑computed terms are kept
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based array of computed terms   */
static idx_t  cache_cap   = 0;       /* current allocated capacity         */
static idx_t  cache_last  = 0;       /* highest index whose value is set   */

/* Ensure cache can hold indices up to 'needed' (inclusive). */
static void grow_cache(idx_t needed)
{
    if (cache_cap > needed) return;

    idx_t newcap = cache_cap ? cache_cap : 16;
    while (newcap <= needed) newcap <<= 1;

    /* Realloc keeps previously stored values. */
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) perror("realloc failed");   /* terminate on allocation error */
    cache = tmp;
    cache_cap = newcap;
}

/* Load the first eight known terms (if they are not already present). */
static void init_base_terms(void)
{
    if (cache_last >= 8) return;

    grow_cache(8);
    cache[1] = (seq_t)4;
    cache[2] = (seq_t)10;
    cache[3] = (seq_t)29;
    cache[4] = (seq_t)89;
    cache[5] = (seq_t)260;
    cache[6] = (seq_t)772;
    cache[7] = (seq_t)2281;
    cache[8] = (seq_t)6741;
    cache_last = 8;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;   /* out‑of‑range guard */
    init_base_terms();

    if (n > cache_last) {
        if (n >= cache_cap) grow_cache(n);

        for (idx_t i = cache_last + 1; i <= n; ++i) {
            seq_t val = 0;
            val += (seq_t)2 * cache[i - 1];
            val += (seq_t)2 * cache[i - 2];
            val += (seq_t)2 * cache[i - 3];
            val += (seq_t)4 * cache[i - 5];
            if (i >= 9) val -= cache[i - 8];
            cache[i] = val;
        }
        cache_last = n;
    }
    return cache[n];
}
