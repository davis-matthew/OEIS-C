/*
 * A390563  Lower (1/5,1/2) midsequence of (3^n) and (5*n); see Comments.
 * https://oeis.org/A390563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 8*a(n-2) + 8*a(n-3) - 8*a(n-4)
   + 8*a(n-5) - 8*a(n-6) + 8*a(n-7) - 8*a(n-8) + 8*a(n-9)
   - 8*a(n-10) + 8*a(n-11) - 7*a(n-12) + 3*a(n-13)
   for n ≥ 13, with
   a(0)=0, a(1)=2, a(2)=5, a(3)=10, a(4)=22,
   a(5)=56, a(6)=155, a(7)=449, a(8)=1325,
   a(9)=3951, a(10)=11826, a(11)=35447, a(12)=106308.
   The sequence is the lower (1/5,1/2) midsequence of (3^n) and (5*n). */

#include <stdlib.h>

/* Shared static cache of computed terms.
   cache_len holds the number of entries stored (index range [0, cache_len-1]). */
static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache contains terms up to index n inclusive.
   The function expands the cache and fills missing values using the recurrence. */
static void ensure_cache(idx_t n)
{
    /* Initial capacity: at least 13 entries for the recurrence base case */
    if (cache_len == 0) {
        /* Allocate 13 entries */
        cache = (seq_t*)malloc(13 * sizeof(seq_t));
        if (!cache) return;            /* allocation failure – caller will get 0 */
        cache_len = 13;
        /* initialise the first thirteen terms */
        cache[0]  = 0ull;
        cache[1]  = 2ull;
        cache[2]  = 5ull;
        cache[3]  = 10ull;
        cache[4]  = 22ull;
        cache[5]  = 56ull;
        cache[6]  = 155ull;
        cache[7]  = 449ull;
        cache[8]  = 1325ull;
        cache[9]  = 3951ull;
        cache[10] = 11826ull;
        cache[11] = 35447ull;
        cache[12] = 106308ull;
    }

    if (cache_len-1 >= n) return;    /* already computed up to n */

    /* Expand the cache to contain indices up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return;                /* allocation failure – caller gets 0 */
    cache = tmp;
    cache_len = new_len;

    /* Compute missing values using the recurrence */
    for (idx_t i = 13; i <= n; ++i) {
        /* local copy of previous terms for clarity; all intermediate values
           fit in seq_t because the recurrence coefficients are small.        */
        seq_t v1  = cache[i-1];
        seq_t v2  = cache[i-2];
        seq_t v3  = cache[i-3];
        seq_t v4  = cache[i-4];
        seq_t v5  = cache[i-5];
        seq_t v6  = cache[i-6];
        seq_t v7  = cache[i-7];
        seq_t v8  = cache[i-8];
        seq_t v9  = cache[i-9];
        seq_t v10 = cache[i-10];
        seq_t v11 = cache[i-11];
        seq_t v12 = cache[i-12];
        seq_t v13 = cache[i-13];
        cache[i] = 5*v1 - 8*v2 + 8*v3 - 8*v4 + 8*v5 - 8*v6 + 8*v7
                   - 8*v8 + 8*v9 - 8*v10 + 8*v11 - 7*v12 + 3*v13;
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Provide 0 for the unlikely case of allocation failure.  */
    if (n < 13 && cache_len) return cache[n];
    ensure_cache(n);
    if (cache_len && n < cache_len) return cache[n];
    return 0;   /* should not happen */
}
