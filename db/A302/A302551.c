/*
 * A302551  a(n) = a(a(n-1)) + a(n-a(n-2)) with a(1) = a(2) = a(5) = 1, a(3) =
 *          a(6) = 2, a(4) = 6.
 * https://oeis.org/A302551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(a(n-1)) + a(n-a(n-2)), a(1)=a(2)=a(5)=1, a(3)=a(6)=2,
 *            a(4)=6
 * Range:     u8 n <= 258, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static size_t cache_cap = 0;

/* Ensure cache has space for indices up to 'n' (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (n < 1) return;
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        assert(cache != NULL);
    }
    while (n > cache_cap) {
        cache_cap = (cache_cap * 2 > cache_cap + 16) ? cache_cap * 2 : cache_cap + 16;
        cache = (seq_t *)realloc(cache, (cache_cap + 1) * sizeof(seq_t));
        assert(cache != NULL);
    }

    /* Fill base values if needed */
    if (cache_len < 6) {
        assert(cache_cap >= 6);
        if (cache_len == 0) { /* first time */
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)1;
            cache[3] = (seq_t)2;
            cache[4] = (seq_t)6;
            cache[5] = (seq_t)1;
            cache[6] = (seq_t)2;
        } else {
            if (cache_len < 1) cache[1] = (seq_t)1;
            if (cache_len < 2) cache[2] = (seq_t)1;
            if (cache_len < 3) cache[3] = (seq_t)2;
            if (cache_len < 4) cache[4] = (seq_t)6;
            if (cache_len < 5) cache[5] = (seq_t)1;
            if (cache_len < 6) cache[6] = (seq_t)2;
        }
        cache_len = 6;
    }

    /* Compute new terms up to 'n' */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t a_nm1 = cache[i - 1];
        seq_t a_nm2 = cache[i - 2];
        idx_t idx1 = (idx_t)a_nm1;
        idx_t idx2 = i - (idx_t)a_nm2;
        assert(idx1 < i && idx2 < i); /* ensure recursion only to smaller indices */
        cache[i] = cache[idx1] + cache[idx2];
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined, but safe */
    ensure_cache(n);
    return cache[n];
}
