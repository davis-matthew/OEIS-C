/*
 * A382616  Expansion of 1/(1 - x/(1 - x)^3)^2.
 * https://oeis.org/A382616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(0)=1, a(1)=2, a(2)=9, a(3)=34, a(4)=124, a(5)=444, a(6)=1567
 *            Recurrence for n>6:
 *            a(n)=8*a(n-1)-22*a(n-2)+26*a(n-3)-17*a(n-4)+6*a(n-5)-a(n-6)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache holding already computed terms   */
static idx_t  cache_last = (idx_t)-1;/* highest index that has been computed   */
static size_t cache_cap  = 0;        /* allocated cache size                   */

/* ensure the cache can contain at least indices 0..n and initialise
   the first seven terms if it is created for the first time */
static void ensure_cache(idx_t n)
{
    if (cache_cap == 0) {
        size_t cap = 7;
        if ((size_t)n + 1 > cap) cap = (size_t)n + 1;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache_cap = cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)34;
        cache[4] = (seq_t)124;
        cache[5] = (seq_t)444;
        cache[6] = (seq_t)1567;
        cache_last = 6;
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        if (new_cap < 7) new_cap = 7;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
        /* newly allocated slots will be filled during expansion */
    }
}

/* compute the n-th term; the function may be called in any order */
seq_t a(idx_t n)
{
    ensure_cache(n);

    if (cache_last < n) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            cache[i] = 8*cache[i-1] - 22*cache[i-2] + 26*cache[i-3]
                       - 17*cache[i-4] + 6*cache[i-5] - cache[i-6];
        }
        cache_last = n;
    }

    return cache[n];
}
