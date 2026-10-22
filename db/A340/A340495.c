/*
 * A340495  Records in first differences of A340494.
 * https://oeis.org/A340495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=2, a(2)=3, a(n)=4*a(n-1)+5*n-19 for n>=3
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

# include <stdlib.h>
# include <stdint.h>


static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of stored terms */
static idx_t  cache_cap = 0;   /* allocated capacity */

/* Ensure the cache can hold at least 'need' terms (1‑based indexing) */
static void
_ensure_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? 2 * cache_cap : 16;
    if (new_cap < (size_t)need)
        new_cap = (size_t)need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* out of memory */
    cache     = tmp;
    cache_cap = (idx_t)new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* safety for non‑positive indices */

    if (n == 1)
        return (seq_t)2;
    if (n == 2)
        return (seq_t)3;

    if (cache_len == 0) {
        _ensure_cache(4);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)8;       /* a(3) */
        cache[3] = (seq_t)33;      /* a(4) */
        cache_len = 4;
    }

    if (n > cache_len)
        _ensure_cache(n);

    for (idx_t i = cache_len; i < n; ++i) {
        idx_t idx = i + 1;                 /* actual term index (1‑based) */
        seq_t val = cache[i - 1] * 4;      /* 4 * a(n-1) */

        if (idx == 3)                     /* 5*3-19 = -4  → subtract 4 */
            val -= (seq_t)4;
        else
            val += (seq_t)(5 * idx - 19); /* adds 5*n-19, positive for n≥4 */

        cache[i] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
