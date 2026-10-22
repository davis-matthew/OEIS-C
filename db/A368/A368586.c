/*
 * A368586  a(n) = n! * Sum_{k=0..n} (-1)^(n-k) * binomial(k+3,4) / k!.
 * https://oeis.org/A368586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 9, i32 n <= 14, i64 n <= 21, i128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A368586
 * a(n) = n!*sum_{k=0..n} (-1)^(n-k)*binomial(k+3,4)/k!
 *       a(0)=0, a(n)= -n*a(n-1) + binomial(n+3,4)
 *   Recurrence with initial value a(0)=0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

/* Dynamic allocation helpers that work in both C and C++ */
#ifdef __cplusplus
static seq_t* allocate_seq(idx_t sz) {
    return new seq_t[sz];          /* constructs objects */
}
static void deallocate_seq(seq_t* p) {
    delete[] p;
}
#else
static seq_t* allocate_seq(idx_t sz) {
    return (seq_t*)malloc(sz * sizeof(seq_t));
}
static void deallocate_seq(seq_t* p) {
    free(p);
}
#endif

/* Cache for computed values */
static seq_t* cache = NULL;
static idx_t cache_size = 0;

/* Return the n-th element of the sequence.  All calls are atomic
   with respect to the static cache. */
seq_t a(idx_t n)
{
    /* Extend the cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;                  /* account for 0..n inclusive */
        seq_t* new_cache = allocate_seq(new_size);

        /* Copy old values to the new array */
        for (idx_t i = 0; i < cache_size; ++i)
            new_cache[i] = cache[i];

        /* Free the old cache */
        if (cache != NULL)
            deallocate_seq(cache);

        cache = new_cache;
        cache_size = new_size;

        /* Compute new entries */
        for (idx_t i = cache_size - 1; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 0;
            } else {
                seq_t bin = binomial(i + 3, 4);
                cache[i] = -(seq_t)i * cache[i - 1] + bin;
            }
        }
    }

    return cache[n];
}
