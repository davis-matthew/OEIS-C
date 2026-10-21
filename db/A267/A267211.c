/*
 * A267211  Number of ON (black) cells in the n-th iteration of the "Rule 109"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) + a(n-4) – a(n-7) for n > 7 with a(0)=1, a(1)=1,
 *            a(2)=5, a(3)=2, a(4)=7, a(5)=7, a(6)=10, a(7)=5
 * Range:     u8 n <= 215, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

/* --- static cache --- */
static seq_t *value_cache = NULL;   /* array of computed terms */
static size_t  cache_cap   = 0;     /* allocated size (in elements) */
static idx_t   cache_len   = 0;     /* number of values currently stored */

/* Ensure the cache can hold indices up to n (inclusive). */
static void
ensure_cache_size(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n + 1)
        new_cap <<= 1;                /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(value_cache,
                                  new_cap * sizeof(seq_t));
    /* In a production environment you would handle realloc failure.
       For the purposes of these problems we assume success. */
    value_cache = tmp;
    cache_cap   = new_cap;
}

/* Initialise the cache with the first eight terms of the sequence. */
static void
init_cache(void)
{
    if (cache_len > 0)
        return;

    ensure_cache_size(7);            /* indices 0 … 7 inclusive */

    value_cache[0] = 1;
    value_cache[1] = 1;
    value_cache[2] = 5;
    value_cache[3] = 2;
    value_cache[4] = 7;
    value_cache[5] = 7;
    value_cache[6] = 10;
    value_cache[7] = 5;

    cache_len = 8;
}

/* Return the n‑th term of the sequence (n ≥ 0). */
seq_t
a(idx_t n)
{
    init_cache();                    /* guarantee initial values */

    if (n < cache_len)               /* value already known */
        return value_cache[n];

    /* Compute all missing values up to n, using the recurrence. */
    ensure_cache_size(n);            /* provide space for all needed entries */

    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 8) {                 /* already initialised */
            continue;
        }
        seq_t sum = value_cache[i-3] + value_cache[i-4];
        value_cache[i] = sum - value_cache[i-7];
    }

    cache_len = n + 1;
    return value_cache[n];
}
