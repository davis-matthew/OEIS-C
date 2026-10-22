/*
 * A317975  a(n) = 2*(a(n-1)+a(n-2)+a(n-3))-a(n-4) for n >= 4, with initial
 *          terms 0, 1, 1, 0.
 * https://oeis.org/A317975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(a(n-1)+a(n-2)+a(n-3)) - a(n-4) with initial values
 *            a(0)=0, a(1)=1, a(2)=1, a(3)=0
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Ensure the cache array holds indices 0..n, reallocating as needed.
   The cast is required for C++ compatibility. */
static void ensure_capacity(idx_t n, seq_t **cache_ptr, size_t *cap_ptr)
{
    size_t needed = (size_t)n + 1;
    if (*cap_ptr < needed) {
        size_t new_cap = *cap_ptr ? *cap_ptr * 2 : 1;
        while (new_cap < needed)
            new_cap <<= 1;
        /* Cast to seq_t* for C++ and C compatibility */
        seq_t *tmp = (seq_t *)realloc(*cache_ptr, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failed */
        *cache_ptr = tmp;
        *cap_ptr  = new_cap;
    }
}

/* Memoized recurrence implementation */
static seq_t *seq_cache = NULL;      /* array of computed values */
static size_t  seq_cap   = 0;        /* allocated capacity */
static idx_t   seq_max   = (idx_t)-1;/* highest index computed */

/* Return the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;

    ensure_capacity(n, &seq_cache, &seq_cap);

    /* Compute any missing values up to n */
    for (idx_t i = seq_max + 1; i <= n; ++i) {
        if (i == 0) seq_cache[0] = (seq_t)0;
        else if (i == 1) seq_cache[1] = (seq_t)1;
        else if (i == 2) seq_cache[2] = (seq_t)1;
        else if (i == 3) seq_cache[3] = (seq_t)0;
        else {
            seq_t t = seq_cache[i-1] + seq_cache[i-2] + seq_cache[i-3];
            t = t + t;                    /* multiply by 2 */
            seq_cache[i] = t - seq_cache[i-4];
        }
    }
    seq_max = n;
    return seq_cache[n];
}
