/*
 * A280668  Number of n X 3 0..2 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors and with new values
 *          introduced in order 0 sequentially upwards.
 * https://oeis.org/A280668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) + 8*a(n-2), a(1)=4, a(2)=59
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;   /* storage array */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* allocated capacity */

/* Ensure that cache has room for at least n elements */
static void
ensure_cache(idx_t n, seq_t **arr, size_t *cap)
{
    if (*cap < (size_t)n) {
        size_t new_cap = (*cap == 0) ? (size_t)n : (*cap * 2);
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* External function requested by the OEIS framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 – behaviour for n<1 is undefined */

    /* Make sure the internal cache can hold all terms up to a(n) */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise first two terms if they are not yet present */
    if (cache_len == 0) {
        cache[0] = (seq_t)4;          /* a(1) */
        cache_len = 1;
    }
    if (cache_len == 1 && n >= 2) {
        cache[1] = (seq_t)59;         /* a(2) */
        cache_len = 2;
    }

    /* Compute any missing terms up to a(n) */
    while (cache_len < (size_t)n) {
        size_t i = cache_len;                     /* i refers to a(i+1) */
        seq_t val = 14 * cache[i - 1] + 8 * cache[i - 2];
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
