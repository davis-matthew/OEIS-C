/*
 * A293122  E.g.f.: exp(-x^2/(1+x)).
 * https://oeis.org/A293122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -(n-1)*( 2*a(n-1) + n*a(n-2) + (n-2)*a(n-3) ) with
 *            a(0)=1, a(1)=0, a(2)=-2. This follows from the differential
 *            equation derived from the EGF exp(-x^2/(1+x))
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The types idx_t (unsigned 64‑bit) and seq_t (signed) are provided by
   the build system.  No redeclaration of them is performed here. */

static seq_t *cache = NULL;  /* cache[i] holds a(i) */
static idx_t computed = 0;   /* largest n computed (0‑based) */

/* Ensure that cache[0..n] are computed. */
static void ensure_cache(idx_t n)
{
    if (n < computed) return;          /* already available */

    /* resize the array if needed */
    size_t new_cap = (size_t)n + 1;
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (cache == NULL) return;       /* allocation failed */
        cache[0] = (seq_t)1;             /* a(0) */
        computed = 0;
    } else if (new_cap > (size_t)computed + 1) {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) return;       /* allocation failed */
    }

    /* Compute missing terms */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t t1 = cache[i-1];                 /* a(i-1) */
        t1 = t1 + t1;                          /* 2*a(i-1) */

        seq_t t2 = cache[i-2];
        t2 = t2 * (seq_t)(i);                  /* n*a(i-2) */

        seq_t t3 = cache[i-3];
        t3 = t3 * (seq_t)(i-2);                /* (n-2)*a(i-3) */

        seq_t sum = t1 + t2 + t3;              /* 2*a(i-1)+n*a(i-2)+(n-2)*a(i-3) */

        seq_t prod = (seq_t)(i-1) * sum;       /* (n-1)*sum */
        cache[i] = -prod;                      /* negative of the product */
    }

    computed = n;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
