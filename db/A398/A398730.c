/*
 * A398730  Upper (-1, 1/2) midsequence of triangular numbers (A000217) and
 *          tetrahedral numbers (A000330); see Comments.
 * https://oeis.org/A398730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 8*a(n-3) - 7*a(n-4) + 4*a(n-5) -
 *            a(n-6), with initial values a(0)=0, a(1)=0, a(2)=0, a(3)=1,
 *            a(4)=5, a(5)=13
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of elements stored in cache */
static size_t cache_cap  = 0;   /* capacity of cache */

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size)
        return;

    /* Grow capacity if necessary. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Base values for the first six terms. */
    static const seq_t base[6] = {0, 0, 0, 1, 5, 13};

    /* Compute missing terms up to n. */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        if (i < 6) {
            cache[i] = base[i];
        } else {
            seq_t s1 = ((seq_t)4 * cache[i-1]) + ((seq_t)8 * cache[i-3]) + ((seq_t)4 * cache[i-5]);
            seq_t s2 = ((seq_t)7 * cache[i-2]) + ((seq_t)7 * cache[i-4]) + cache[i-6];
            cache[i] = s1 - s2;   /* s1 >= s2 for this recurrence */
        }
    }
    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
