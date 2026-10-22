/*
 * A304620  Expansion of (1/(1 - x)) * Sum_{k>=0} x^(2*k) / Product_{j=1..2*k}
 *          (1 - x^j).
 * https://oeis.org/A304620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = output of OEIS A304620, defined by G(x) = (1/(1 - x)) *
 *            Σ_{k≥0} x^{2k} / ∏_{j=1}^{2k} (1 - x^j)
 * Range:     u8 n <= 14, u16 n <= 37, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm builds the partition function f[s] for parts up to m
   and uses it to accumulate the required sums.  No intermediate value
   exceeds that needed for the final result, so it works for the
   64‑bit build for all n whose a(n) fits 64 bits. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate all arrays of size n+1.  The temporary array 'pref'
       holds the cumulative sums of f for the current part size. */
    idx_t N = n;
    seq_t *f    = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *pref = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *res  = (seq_t *)malloc((N + 1) * sizeof(seq_t));

    if (!f || !pref || !res) {
        /* In practice this should never happen for the limits
           considered; on failure return zero. */
        free(f); free(pref); free(res);
        return (seq_t)0;
    }

    /* Initialise */
    f[0] = (seq_t)1;
    for (idx_t i = 1; i <= N; ++i) f[i] = (seq_t)0;
    for (idx_t i = 0; i <= N; ++i) res[i] = (seq_t)0;

    /* Main loop over the maximal allowed part size m. */
    for (idx_t m = 1; m <= N; ++m) {
        /* Update partition counts for part size m. */
        for (idx_t s = m; s <= N; ++s) {
            f[s] = f[s] + f[s - m];
        }

        /* When m is even, add contributions to the result.      */
        if ((m & 1ULL) == 0) {
            /* Build prefix sums of f (i.e., sums of partitions up to
               each size).  pref[t] = Σ_{i=0}^{t} f[i].             */
            pref[0] = f[0];
            for (idx_t i = 1; i <= N; ++i) {
                pref[i] = pref[i - 1] + f[i];
            }

            /* Each even part size m = 2k contributes:
               pref[n - m] to a[n]  for all n >= m.                */
            for (idx_t nidx = m; nidx <= N; ++nidx) {
                res[nidx] = res[nidx] + pref[nidx - m];
            }
        }
    }

    /* Add the k = 0 contribution (S(0, n) = 1 for all n).        */
    for (idx_t i = 0; i <= N; ++i) {
        res[i] = res[i] + (seq_t)1;
    }

    seq_t answer = res[N];

    free(f); free(pref); free(res);
    return answer;
}
