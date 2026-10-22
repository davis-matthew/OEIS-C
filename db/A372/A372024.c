/*
 * A372024  Expansion of ( (1 + 8*x)/(1 - x) )^(1/3).
 * https://oeis.org/A372024
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 11, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A372024: Expansion of ( (1 + 8*x)/(1 - x) )^(1/3).
   Recurrence: a(0)=1, a(1)=3,
   n*a(n) + (7n-10)*a(n-1) + 8*(-n+2)*a(n-2) = 0  for n>=2. */
#include <stdlib.h>

static seq_t *cache = NULL;     /* Array of computed terms */
static idx_t cache_size = 0;    /* Number of terms currently in cache */
static size_t cache_capacity = 0; /* Allocated capacity of cache */

seq_t a(idx_t n) {
    /* Handle the first two terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Initial cache allocation. */
    if (cache == NULL) {
        cache_capacity = (size_t)2;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache_size = 2;
    }

    /* Grow cache if necessary to hold term n. */
    if ((size_t)n + 1 > cache_capacity) {
        cache_capacity = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t n_seq = (seq_t)i;

        /* numerator = -((7*n-10)*prev1 + 8*(2-n)*prev2) */
        seq_t term1 = (seq_t)7 * n_seq - (seq_t)10;
        seq_t term2 = (seq_t)8 * ((seq_t)2 - n_seq);
        seq_t numerator = - (term1 * prev1 + term2 * prev2);

        cache[i] = numerator / n_seq;   /* division is exact by theory */
    }

    cache_size = n + 1;
    return cache[n];
}
