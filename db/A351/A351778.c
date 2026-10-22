/*
 * A351778  Expansion of e.g.f. 1/(1 + 3*x*exp(x)).
 * https://oeis.org/A351778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351778: Expansion of e.g.f. 1/(1 + 3*x*exp(x))
   Recurrence: a(0) = 1
                a(n) = -3 * n * Σ_{k=0}^{n-1} binomial(n-1,k) * a(k)   (n > 0)
   The implementation memoises all computed values so that calls can arrive
   in any order.  It is O(n²) time and O(n) space for computing up to n.
   All arithmetic is performed with the arbitrary / fixed size seq_t type
   supplied by the build, and no floating point operations are used. */
#include <stdlib.h>

/* --------- Local binomial helper ------------------------------------- */
/* Computes combinatorial coefficient C(n,k) for idx_t arguments.
   The result is stored in type seq_t; division by a small integer is
   allowed by the build's integer type. */
static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n)          /* should not happen, but guard */
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i  */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* --------- Memoisation cache --------------------------------------- */
static seq_t *a_cache = NULL;   /* array holding a[0]..a[a_cached-1] */
static idx_t a_cached = 0;      /* current number of cached values */

/* Ensure that the cache contains values up to index n.  New values are
   computed using the recurrence.  The routine is thread‑safe only in
   the sense that it does not modify global state between calls. */
static void ensure_a_cache(idx_t n)
{
    if (n < a_cached)          /* already computed */
        return;

    /* Allocate or grow the array to hold up to index n */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    } else {
        a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    }

    /* Compute missing values */
    for (idx_t i = a_cached; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;     /* a(0) = 1 */
            continue;
        }
        seq_t sum = 0;
        idx_t m = i - 1;
        for (idx_t k = 0; k < i; ++k) {
            seq_t term = binom_idx(m, k);
            term = term * a_cache[k];
            sum = sum + term;
        }
        a_cache[i] = sum * (seq_t)(-3) * (seq_t)i;
    }

    a_cached = n + 1;
}

/* --------- Public API ---------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_a_cache(n);
    return a_cache[n];
}
