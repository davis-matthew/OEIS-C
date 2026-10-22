/*
 * A348314  a(n) = n! * Sum_{k=0..n-1} 4^k / k!.
 * https://oeis.org/A348314
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = n * (a(n-1) + 4^(n-1)) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */

/* ------------------------------------------------------------------ */
/* Static cache and state                                            */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;        /* array of computed a(i)            */
static size_t cache_capacity = 0;  /* number of seq_t elements allocated */
static idx_t  computed_up_to = 0;  /* indices < computed_up_to are ready */
static seq_t  pow4 = (seq_t)1;     /* 4^i where i = computed_up_to      */

/* ------------------------------------------------------------------ */
/* Ensure that a(0)..a(n) are in 'cache'                              */
/* ------------------------------------------------------------------ */
static void ensure_up_to(idx_t n)
{
    if (n < computed_up_to)                /* already available */
        return;

    /* Grow the storage if necessary --------------------------------*/
    size_t new_capacity = cache_capacity ? cache_capacity : 8;
    while (new_capacity <= (size_t)n)
        new_capacity <<= 1;

    /* Use a cast so the code works in C++.  In C the cast is optional. */
    seq_t *new_cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (new_cache == NULL)                  /* out of memory */
        abort();

    cache = new_cache;
    cache_capacity = new_capacity;

    /* Compute missing terms ----------------------------------------*/
    for (idx_t i = computed_up_to; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)0;          /* a(0) = 0                 */
            pow4      = (seq_t)1;          /* 4^0 for the next step    */
        } else {
            seq_t val = cache[i-1] + pow4; /* a(i-1) + 4^(i-1)         */
            val = val * (seq_t)i;         /* * i                       */
            cache[i] = val;

            pow4 = pow4 * (seq_t)4;       /* advance to 4^i for next  */
        }
    }

    computed_up_to = n + 1;                /* next index that is still uncomputed */
}

/* ------------------------------------------------------------------ */
/* Public interface (to be called by the evaluation harness)          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
