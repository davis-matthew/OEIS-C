/*
 * A277953  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 14", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A277953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1)+a(n-2)-10*a(n-3) for n>2, with initial values
 *            a(0)=1, a(1)=11, a(2)=11
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached sequence values (static to preserve across calls). */
static seq_t *cache      = NULL;   /* array of computed terms   */
static idx_t  cache_len  = 0;      /* number of terms stored   */
static idx_t  cache_cap  = 0;      /* capacity of cache array  */

/* Ensure that cache[n] is available, computing any missing terms. */
static void ensure_cache(idx_t n)
{
    /* Ensure the buffer is large enough. */
    if (cache_cap <= n) {
        idx_t new_cap = cache_cap ? cache_cap : 3;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first three terms once. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)11;
        cache[2] = (seq_t)11;
        cache_len = 3;
    }

    /* Compute any terms that are not yet cached. */
    if (n + 1 > cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t t = cache[i-1] * (seq_t)10; /* 10*a(i-1) */
            t += cache[i-2];                 /* + a(i-2)  */
            t -= cache[i-3] * (seq_t)10;     /* -10*a(i-3) */
            cache[i] = t;
        }
        cache_len = n + 1;
    }
}

/* Return the n-th term of A277953. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
