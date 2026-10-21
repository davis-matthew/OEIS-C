/*
 * A294044  a(0) = 0, a(1) = a(2) = 1; a(2*n) = 2*a(n) + a(n+1), a(2*n+1) =
 *          a(n) + a(n+1).
 * https://oeis.org/A294044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = a(2) = 1; a(2*n) = 2*a(n) + a(n+1), a(2*n+1) =
 *            a(n) + a(n+1) (for n > 1)
 * Range:     u8 n <= 79, u16 n <= 4863, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache holding the computed values.
   a_cache[i] contains a(i); cache grows as needed. */
static seq_t *a_cache = NULL;
static idx_t  a_cache_size = 0;

/* Grow the cache to ensure it has room for indices 0..new_size-1.
   Newly added elements are computed using the recurrence. */
static void ensure_cache(idx_t new_size)
{
    if (new_size <= a_cache_size)
        return;                       /* already big enough */

    /* Allocate or reallocate storage.  Cast the void* returned by realloc
       so the code compiles as C++ as well. */
    size_t new_cap = (size_t)new_size;
    seq_t *new_cache = (seq_t *)realloc(a_cache,
                                         new_cap * sizeof(seq_t));
    if (!new_cache) {
        abort();                      /* allocation failed */
    }

    /* Compute values for all new indices. */
    for (idx_t i = a_cache_size; i < new_size; ++i) {
        if (i == 0) {
            new_cache[i] = (seq_t)0;
        } else if (i == 1 || i == 2) {
            new_cache[i] = (seq_t)1;
        } else if ((i & 1) == 0) {            /* i even : i = 2*n */
            idx_t k = i >> 1;                 /* k = i/2   */
            new_cache[i] = (seq_t)( (seq_t)2 * new_cache[k]
                                    + new_cache[k + 1] );
        } else {                               /* i odd  : i = 2*n+1 */
            idx_t k = (i - 1) >> 1;            /* k = (i-1)/2 */
            new_cache[i] = new_cache[k]
                          + new_cache[k + 1];
        }
    }

    a_cache = new_cache;
    a_cache_size = new_size;
}

/* Public entry point.  Computes a(n) for any n >= 0. */
seq_t a(idx_t n)
{
    ensure_cache(n + 1);   /* guarantee a_cache[n] exists */
    return a_cache[n];
}
