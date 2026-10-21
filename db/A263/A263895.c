/*
 * A263895  Expansion of e.g.f.: exp(-x)*x/(1-2*x)^2.
 * https://oeis.org/A263895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 0, a(1) = 1, a(2) = 6;
   a(n) = 2*(n+1)*a(n-1) + 3*a(n-2) + 2*(2-n)*a(n-3)   (n >= 3) */

#include <stdlib.h>

static seq_t *cache = NULL;  /* dynamic array holding computed terms */
static size_t  cap  = 0;     /* current capacity of the array */
static idx_t   last = 0;     /* index of the last computed value */

seq_t a(idx_t n)
{
    /* Base cases can be returned immediately without allocating memory. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)6;

    /* Grow the array if necessary. */
    if ((idx_t)n >= (idx_t)cap) {
        size_t new_cap = (size_t)n + 2;          /* at least n+1 elements */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first three values if we have not done so yet. */
    if (last < 2) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)6;
        last = 2;
    }

    /* Compute terms that have not yet been computed. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t1 = ((seq_t)2 * (seq_t)(i + 1)) * cache[i - 1];   /* 2*(i+1)*a(n-1) */
        seq_t t2 = (seq_t)3 * cache[i - 2];                      /* 3*a(n-2) */
        seq_t t3 = ((seq_t)2 * (seq_t)(i - 2)) * cache[i - 3];   /* 2*(i-2)*a(n-3) */
        cache[i] = t1 + t2 - t3;                                 /* a(n) = t1 + t2 - t3 */
    }
    last = n;   /* update the highest computed index */

    return cache[n];
}
