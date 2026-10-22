/*
 * A359942  Expansion of Product_{k>=0} (1 - x^(k^3+1)) in powers of x.
 * https://oeis.org/A359942
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 87, i16 n <= 5954, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A359942: Expansion of Product_{k>=0} (1 - x^(k^3+1)) in powers of x.
 *
 *  Recurrence (OEIS notation):
 *      a(0) = 1
 *      a(n) = -(1/n) * Σ_{k=1..n} A359943(k) * a(n-k)
 *
 *  where A359943(k) = Σ_{d∈S , d|k} d and S = {t^3+1 : t ≥ 0}.
 *
 *  This implementation computes the divisor sums by a simple sieve
 *  and then evaluates the recurrence with memoisation.
 */

#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------- */
/*  Static helper state                                                  */
/* --------------------------------------------------------------------- */
static seq_t *c_array  = NULL;                    /* A359943(k) values          */
static idx_t  c_size   = 0;                       /* allocated length of c_array*/
static seq_t *a_array  = NULL;                    /* a(n) values                */
static idx_t  a_size   = 0;                       /* allocated length of a_array*/
static idx_t  last_a   = 0;                       /* highest index already computed */

/* --------------------------------------------------------------------- */
/*  Compute A359943(k) for all k ≤ n (recomputes from scratch).          */
/* --------------------------------------------------------------------- */
static void compute_c(idx_t n)
{
    /* Allocate / enlarge storage if needed. */
    if (c_size <= n) {
        seq_t *tmp = (seq_t *)realloc(c_array, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        c_array = tmp;
        c_size  = n + 1;
    }

    /* Zero the array. */
    memset(c_array, 0, (n + 1) * sizeof(seq_t));

    /* Sieve: for each d = t^3+1 ≤ n, add d to all multiples of d. */
    for (idx_t t = 0; ; ++t) {
        idx_t d = t * t * t + 1ULL;        /* t^3+1 fits into idx_t for |n| ≤ 10^4 */
        if (d > n) break;
        for (idx_t m = d; m <= n; m += d) {
            c_array[m] += (seq_t)d;
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Compute a(n) on demand, using memoisation.                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Ensure the divisor-sum table is ready. */
    compute_c(n);

    /* Allocate / enlarge storage for a_array if needed. */
    if (a_size <= n) {
        seq_t *tmp = (seq_t *)realloc(a_array, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        a_array = tmp;
        a_size  = n + 1;
    }

    /* Initialise base case a(0) = 1 on first use. */
    if (last_a == 0 && a_array[0] == 0) {
        a_array[0] = (seq_t)1;
        last_a = 0;
    }

    /* If the requested term has already been computed, return it. */
    if (last_a >= n) return a_array[n];

    /* Compute missing terms up to n. */
    for (idx_t m = last_a + 1; m <= n; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            sum += c_array[k] * a_array[m - k];
        }
        a_array[m] = -(sum / (seq_t)m);
    }

    last_a = n;
    return a_array[n];
}
