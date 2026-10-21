/*
 * A269676  Number of length-n 0..6 arrays with no repeated value differing
 *          from the previous repeated value by other than plus or minus one
 *          modulo 6+1.
 * https://oeis.org/A269676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 11*a(n-1) - 22*a(n-2) - 48*a(n-3) with initial values
 *            a(1) = 7, a(2) = 49, a(3) = 336
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;
static size_t a_cache_size = 0;   /* highest index computed (1‑based) */
static size_t a_cache_cap  = 0;   /* allocated capacity of a_cache */

/* Ensure the cache can hold values up to index n (inclusive). */
static void
ensure_a_cache(idx_t n)
{
    if (a_cache_size >= n)
        return;

    /* Grow the cache if needed. */
    if (a_cache_cap == 0)
        a_cache_cap = 16;                      /* initial capacity */
    while (a_cache_cap <= (size_t)n)
        a_cache_cap *= 2;

    a_cache = (seq_t *)realloc(a_cache,
                               a_cache_cap * sizeof(seq_t));
    if (a_cache == NULL)
        abort();  /* Out of memory – stop execution. */

    /* Initialize cached values from a_cache_size+1 up to n. */
    for (size_t i = a_cache_size + 1; i <= (size_t)n; ++i) {
        if (i == 1)
            a_cache[i] = (seq_t)7;
        else if (i == 2)
            a_cache[i] = (seq_t)49;
        else if (i == 3)
            a_cache[i] = (seq_t)336;
        else {
            /* 11*a(i-1) - 22*a(i-2) - 48*a(i-3) */
            seq_t term = a_cache[i - 1] * (seq_t)11
                       - a_cache[i - 2] * (seq_t)22
                       - a_cache[i - 3] * (seq_t)48;
            a_cache[i] = term;
        }
    }
    a_cache_size = (size_t)n;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* value not defined for n < 1 */

    ensure_a_cache(n);
    return a_cache[(size_t)n];
}
