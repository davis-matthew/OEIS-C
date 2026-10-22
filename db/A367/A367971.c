/*
 * A367971  Expansion of e.g.f. exp(exp(-x) - 1)/(1 - x).
 * https://oeis.org/A367971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Sum_{k=1..n} ((k-1)! + (-1)^k) *
 *            binomial(n-1,k-1) * a(n-k) (n >= 1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The following types are provided by the build system.
   seq_t : unsigned integer type (possibly arbitrary‑precision).
   idx_t : unsigned integer type for indices (UInt64). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate arrays.  Each element is allocated as an uninitialised
       seq_t.  For integer types this is fine; for arbitrary‑precision
       types the build system has to provide a trivial constructor
       that will be called by malloc. */
    seq_t *fact = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *C    = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *arr  = (seq_t *)calloc((n + 1), sizeof(seq_t)); /* a[0..n] */

    if (!fact || !C || !arr) { /* memory failure – abort */
        if (fact) free(fact);
        if (C)    free(C);
        if (arr)  free(arr);
        return (seq_t)0;
    }

    /* Factorials: fact[0] = 1, fact[i] = i!* */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Coefficients C[k] = (k-1)! + (-1)^k */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t f = fact[k - 1];
        if (k & 1)        /* odd k: (-1)^k = -1 */
            C[k] = f - (seq_t)1;
        else              /* even k: (-1)^k = +1 */
            C[k] = f + (seq_t)1;
    }
    arr[0] = (seq_t)1;

    /* Compute a[n] via the recurrence */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;   /* binom(m-1,0) */

        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = C[k] * binom * arr[m - k];
            sum += term;

            if (k < m) {
                /* Update binom to binom(m-1,k) */
                seq_t num = binom * (seq_t)(m - k);
                binom = num / (seq_t)k;   /* exact division */
            }
        }
        arr[m] = sum;
    }

    seq_t result = arr[n];

    /* Clean up */
    free(fact);
    free(C);
    free(arr);

    return result;
}
