/*
 * A308673  For n > 2, if there exists an m < n such that a(m) = a(n), take
 *          the largest such m and set a(n+1) = a(n)+(n-m); otherwise a(n+1) =
 *          1. Start with a(1)=0, a(2)=0.
 * https://oeis.org/A308673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=0, a(3)=1. For n>2, let m be the largest index < n
 *            with a(m)=a(n). Then a(n+1)=a(n)+(n-m) if such m exists,
 *            otherwise a(n+1)=1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note:
 * The function is memoised: it supports any order of calls.
 * The storage is 1‑indexed, so a(1) is stored at cache[1].
 */

#include <stdlib.h>   /* malloc, free, abort */
#include <stddef.h>   /* size_t */

static seq_t *cache = NULL;   /* 1‑indexed cache */
static size_t cache_cap = 0;  /* allocated capacity (excluding unused index 0) */
static idx_t cache_size = 0;  /* number of terms currently stored (>=0) */

/* Ensure that the cache can hold indices up to n. */
static void
ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;   /* double or start at 4 */
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *new_data = (seq_t*)malloc((new_cap + 1) * sizeof(seq_t));
    if (!new_data) abort();           /* out of memory */
    if (cache) {                       /* copy existing entries */
        for (idx_t i = 1; i <= cache_size; ++i)
            new_data[i] = cache[i];
        free(cache);
    }
    cache = new_data;
    cache_cap = new_cap;
}

/* Compute a(n) for any n >= 1.  */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined index, return 0 to avoid UB */
        return 0;
    if (n == 1 || n == 2)
        return 0;
    if (n == 3)
        return 1;

    ensure_cache(n);

    /* initialise first three terms if not already present */
    if (cache_size < 3) {
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache_size = 3;
    }

    /* generate missing terms up to n */
    for (idx_t i = cache_size; i < n; ++i) {
        seq_t val = cache[i];
        idx_t m = 0;                 /* last occurrence of val */
        for (idx_t j = i - 1; j != 0; --j) {
            if (cache[j] == val) {
                m = j;
                break;
            }
        }
        if (m)
            cache[i + 1] = val + (seq_t)(i - m);
        else
            cache[i + 1] = 1;
    }
    cache_size = n;
    return cache[n];
}
