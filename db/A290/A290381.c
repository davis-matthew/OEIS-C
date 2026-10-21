/*
 * A290381  Number of minimal dominating sets in the n-web graph.
 * https://oeis.org/A290381
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)+5*a(n-3)+4*a(n-4)+4*a(n-5)-8*a(n-6) for n
 *            > 8, a(3)=22, a(4)=53, a(5)=146, a(6)=338, a(7)=995, a(8)=2661
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache   = NULL;   /* cache array */
static size_t cache_len   = 0;      /* allocated length */
static idx_t  cache_max   = 0;      /* highest n computed so far */

/* Ensure the cache can hold indices up to n */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)(n + 1);    /* indices 0..n inclusive */
    if (cache_len >= need) return;

    void *tmp = realloc(seq_cache, need * sizeof(seq_t));
    if (tmp == NULL) {               /* allocation failed */
        free(seq_cache);
        seq_cache = NULL;
        cache_len = 0;
        cache_max = 0;
        return;
    }
    seq_cache = (seq_t *)tmp;
    cache_len = need;
}

/* Base values for indices 3 through 8 */
static const seq_t base_vals[6] = {
    (seq_t)22,
    (seq_t)53,
    (seq_t)146,
    (seq_t)338,
    (seq_t)995,
    (seq_t)2661
};

seq_t a(idx_t n)
{
    /* values below the offset are not defined */
    if (n < 3) return (seq_t)0;

    ensure_cache(n);
    if (seq_cache == NULL) return (seq_t)0;   /* allocation failure */

    /* If we have already computed up to n, just return it */
    if (cache_max >= n) return seq_cache[n];

    /* Initialize base values if needed */
    if (cache_max < 3) {
        idx_t end = (n > 8 ? 8 : n);
        for (idx_t i = 3; i <= end; ++i)
            seq_cache[i] = base_vals[i - 3];
        cache_max = end;
    } else if (cache_max < 8) {
        idx_t end = (n > 8 ? 8 : n);
        for (idx_t i = cache_max + 1; i <= end; ++i)
            seq_cache[i] = base_vals[i - 3];
        cache_max = end;
    }

    /* Compute remaining terms using the recurrence */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val =
            seq_cache[i - 1]
          + (seq_t)2 * seq_cache[i - 2]
          + (seq_t)5 * seq_cache[i - 3]
          + (seq_t)4 * seq_cache[i - 4]
          + (seq_t)4 * seq_cache[i - 5]
          - (seq_t)8 * seq_cache[i - 6];
        seq_cache[i] = val;
    }

    cache_max = n;
    return seq_cache[n];
}
