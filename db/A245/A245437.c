/*
 * A245437  Expansion of x^5/(x^6-x^4-x^2-x+1).
 * https://oeis.org/A245437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) - a(n-6) (n > 5) Initial values:
 *            a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=1
 * Range:     u8 n <= 16, u16 n <= 26, u32 n <= 46, u64 n <= 88, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached sequence storage */
static seq_t *cache = NULL;    /* dynamic array of computed terms   */
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* current capacity of the array     */

static const seq_t base_values[6] = {
    (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1
};

/* Ensure the cache contains terms up to index n (inclusive) */
static void ensure_cache(idx_t n)
{
    /* Initialize cache on first use */
    if (cache == NULL) {
        cache_cap = 64;                   /* arbitrary starting capacity */
        cache_len = 6;                    /* we have 6 predefined values */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 6; ++i) {
            cache[i] = base_values[i];
        }
    }

    /* Expand the cache array if needed */
    while (cache_len <= (size_t)n) {
        if (cache_len == cache_cap) {
            /* double the capacity */
            size_t new_cap = cache_cap * 2;
            seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (new_cache == NULL) {
                /* In a real application we might handle the error; here we just abort */
                abort();
            }
            cache = new_cache;
            cache_cap = new_cap;
        }

        /* Compute the next term using the recurrence */
        seq_t term = cache[cache_len - 1] + /* a(n-1)   */
                     cache[cache_len - 2] + /* a(n-2)   */
                     cache[cache_len - 4]   /* a(n-4)   */
                     - cache[cache_len - 6];/* a(n-6)   */
        cache[cache_len] = term;
        cache_len++;
    }
}

seq_t a(idx_t n)
{
    if (n < 6) {
        /* Return the hard‑coded initial values directly */
        return base_values[(size_t)n];
    }

    ensure_cache(n);
    return cache[(size_t)n];
}
