/*
 * A386230  G.f. A(x) satisfies A(x) = 1/( (1-x)^3 * (1 - x*A(x) - x^2*A'(x))
 *          ).
 * https://oeis.org/A386230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A386230:  a(n) = binomial(n+2,2) + (n+1)/2 * Σ_{k=0}^{n-1} a(k) * a(n-1-k)
   a(0) = 1 */

#include <stdlib.h>      /* for malloc, realloc */

static seq_t *a_cache = NULL;  /* memoisation array               */
static size_t a_cap   = 0;      /* current capacity of a_cache     */
static idx_t  a_len   = 0;      /* number of values already computed */

/* Return a(n) for any n ≥ 0.  Calls may occur in arbitrary order.  */
seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /* 1. Initialise cache if this is the first call.                     */
    /* ------------------------------------------------------------------ */
    if (a_cache == NULL) {
        a_cap = 1;
        a_cache = (seq_t *)realloc(NULL, a_cap * sizeof(seq_t));
        if (a_cache == NULL) return (seq_t)0;   /* allocation failed */
        a_cache[0] = (seq_t)1;                  /* a(0) = 1            */
        a_len   = 1;
    }

    /* ------------------------------------------------------------------ */
    /* 2. Resize cache so that it can hold indices up to n.               */
    /* ------------------------------------------------------------------ */
    if ((idx_t)(n + 1) > (idx_t)a_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        a_cache = tmp;
        /* initialise newly added entries to zero */
        for (size_t i = a_cap; i < new_cap; ++i)
            a_cache[i] = (seq_t)0;
        a_cap = new_cap;
    }

    /* ------------------------------------------------------------------ */
    /* 3. Compute all missing terms up to n.                               */
    /* ------------------------------------------------------------------ */
    for (idx_t i = a_len; i <= n; ++i) {
        /* Σ_{k=0}^{i-1} a(k) * a(i-1-k)  (convolution) */
        seq_t s = (seq_t)0;
        for (idx_t k = 0; k < i; ++k) {
            s += a_cache[k] * a_cache[i - 1 - k];
        }

        /* binomial(i+2,2) = (i+2)*(i+1)/2 */
        seq_t binom = ((seq_t)(i + 2)) * ((seq_t)(i + 1)) / 2;

        /* extra term ((i+1)*s)/2 */
        seq_t extra = ((seq_t)(i + 1)) * s / 2;

        a_cache[i] = binom + extra;
    }
    a_len = n + 1;

    /* ------------------------------------------------------------------ */
    /* 4. Return the requested value.                                     */
    /* ------------------------------------------------------------------ */
    return a_cache[n];
}
