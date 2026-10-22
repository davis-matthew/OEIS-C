/*
 * A357291  a(n) = number of subsets S of {1,2,...,n} having more than 2
 *          elements such that (sum of least two elements of S) < difference
 *          between greatest two elements of S.
 * https://oeis.org/A357291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 2*a(n-3) - a(n-4) + a(n-5) + 3*a(n-6)
 *            - 2*a(n-7) (a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0,a(5)=0,a(6)=1)
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[7] = {0,0,0,0,0,0,1};
static seq_t *cache = NULL;      /* dynamically grown array of sequence terms   */
static idx_t cache_len = 0;      /* number of terms currently stored in cache */

/* Ensure the cache is large enough for indices up to n, and pre‑compute
   any missing terms using the recurrence.  */
static void ensure_cache(idx_t n)
{
    /* Initial allocation */
    if (cache == NULL) {
        size_t init = (size_t) (n + 1);
        if (init < 7) init = 7;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (cache == NULL) exit(1);     /* memory allocation failed */
        /* copy the first seven values */
        for (idx_t i = 0; i < 7 && i <= n; ++i)
            cache[i] = base_vals[i];
        cache_len = (n + 1 > 7) ? 7 : (n + 1);
    }

    /* Grow if necessary */
    if (cache_len <= n) {
        size_t new_cap = (size_t) ((cache_len > 0) ? cache_len * 2 : 8);
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            free(cache);
            exit(1);
        }
        cache = tmp;
        /* compute terms already known (from cache_len onward) */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 3 * cache[i - 1]
                        - cache[i - 2]
                        - 2 * cache[i - 3]
                        - cache[i - 4]
                        + cache[i - 5]
                        + 3 * cache[i - 6]
                        - 2 * cache[i - 7];
            cache[i] = val;
        }
        cache_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n < 7) return base_vals[n];
    ensure_cache(n);
    return cache[n];
}
