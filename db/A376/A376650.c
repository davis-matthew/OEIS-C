/*
 * A376650  a(n) = Sum_{k=0..floor(n/4)} binomial(floor(k/3),n-4*k).
 * https://oeis.org/A376650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A376650
   a(n) = a(n-12) + a(n-13)   (n ≥ 12)
   a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=0, a(7)=0,
   a(8)=1, a(9)=0, a(10)=0, a(11)=0, a(12)=1, a(13)=1
   (the first 14 terms are given explicitly).  */

#include <stddef.h>
#include <stdlib.h>

static const seq_t base_vals[14] = {
    1,0,0,0,1,0,0,0,1,0,0,0,1,1
};

/* Cached values for already computed terms */
static seq_t *cache = NULL;     /* array of a(i) values */
static size_t cache_len = 0;    /* number of values stored in cache */
static size_t cache_cap = 0;    /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* Handle the first 14 terms directly. */
    if (n < 14) {
        return base_vals[(size_t)n];
    }

    /* Ensure cache has enough capacity for index n. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        /* Allocate or grow cache. */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        /* In case of allocation failure, abort. */
        if (cache == NULL) {
            abort();
        }
        cache_cap = new_cap;
    }

    /* Initiate base values if they have not yet been stored. */
    if (cache_len < 14) {
        for (size_t i = cache_len; i < 14; ++i) {
            cache[i] = base_vals[i];
        }
        cache_len = 14;
    }

    /* Compute all terms up to n using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 12] + cache[i - 13];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
