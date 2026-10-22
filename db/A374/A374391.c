/*
 * A374391  Number of ways to tile a hexagonal strip made up of n equilateral
 *          triangles, using diamonds and trapezoids.
 * https://oeis.org/A374391
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-4) + 2*a(n-12) + 3*a(n-16) - a(n-20) for n ≥ 20
 *            with initial values a(-1)=0, a(0)=1, a(1)=0, a(2)=1, a(3)=1,
 *            a(4)=1, a(5)=2, a(6)=5, a(7)=4, a(8)=4, a(9)=6, a(10)=22,
 *            a(11)=17, a(12)=17, a(13)=25, a(14)=89, a(15)=70, a(16)=73,
 *            a(17)=104, a(18)=369, a(19)=291 (values up to a(19) initi...
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern void free_seq_array(seq_t *p); /* optional cleanup helper */

static const seq_t base_vals[21] = {
    0,  /* a(-1) */
    1,  /* a(0)  */
    0,  /* a(1)  */
    1,  /* a(2)  */
    1,  /* a(3)  */
    1,  /* a(4)  */
    2,  /* a(5)  */
    5,  /* a(6)  */
    4,  /* a(7)  */
    4,  /* a(8)  */
    6,  /* a(9)  */
    22, /* a(10) */
    17, /* a(11) */
    17, /* a(12) */
    25, /* a(13) */
    89, /* a(14) */
    70, /* a(15) */
    73, /* a(16) */
    104,/* a(17) */
    369,/* a(18) */
    291 /* a(19) */
};

static seq_t *cache = NULL;        /* cache[n+1] stores a(n)  */
static size_t cache_size = 0;      /* number of entries allocated */

/* Compute a(n), with n ≥ -1. */
seq_t a(idx_t n)
{
    /* Handle the only negative index supported. */
    if (n == -1) return base_vals[0];

    /* Initialise the cache on first call. */
    if (cache == NULL) {
        /* We store values for n = -1 .. 19 : 21 entries. */
        cache_size = 21;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (cache == NULL) { perror("calloc"); abort(); }
        for (size_t i = 0; i < 21; ++i) {
            cache[i] = base_vals[i];
        }
    }

    /* If n is small, look directly into the base table. */
    if ((size_t)n + 1 < cache_size) {
        return cache[(size_t)n + 1];
    }

    /* Need to compute further terms up to index n. */
    size_t target_index = (size_t)n + 1;          /* index in the array for a(n) */
    size_t new_size    = target_index + 1;        /* required array length */

    /* Resize cache if necessary. */
    if (new_size > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) { perror("realloc"); abort(); }
        cache = new_cache;

        /* Compute new values using the linear recurrence. */
        for (size_t i = cache_size; i < new_size; ++i) {
            /* Recurrence: a(i-1) = 4*a(i-5) + 2*a(i-13) + 3*a(i-17) - a(i-21) */
            cache[i] = 4 * cache[i - 4] + 2 * cache[i - 12] + 3 * cache[i - 16] - cache[i - 20];
        }
        cache_size = new_size;
    }

    return cache[target_index];
}
