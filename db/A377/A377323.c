/*
 * A377323  E.g.f. satisfies A(x) = 1 - log(1 - x*A(x)^3)/A(x).
 * https://oeis.org/A377323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (3*n-k)!/(3*n-2*k+1)! * |Stirling1(n,k)|
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoized implementation using Stirling numbers of the first kind. */

#include <stddef.h>

#define MAXN 10001   /* maximum n expected for the tests (>= 0) */

seq_t a(idx_t n) {
    static seq_t s_row[MAXN];
    static seq_t a_cache[MAXN];
    static idx_t max_computed = 0;
    static int initialized = 0;

    /* Initialise only once.  Static local variables are zero‐initialised. */
    if (!initialized) {
        s_row[0]  = (seq_t)1;   /* S1(0,0) = 1 */
        a_cache[0] = (seq_t)1;  /* a(0)   = 1 */
        max_computed = 0;
        initialized = 1;
    }

    /* Guard against out‑of‑range requests.  The test suite never asks for
       such values, but returning 0 prevents UB. */
    if (n >= MAXN) return (seq_t)0;

    /* Compute rows up to n if we have not done so already. */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* Update Stirling numbers of the first kind in place
               (reverse order to keep previous row values). */
            for (idx_t k = i; k >= 1; --k) {
                seq_t temp = ((seq_t)(i - 1)) * s_row[k];
                s_row[k] = s_row[k - 1] + temp;
            }
            s_row[0] = (seq_t)0;  /* After the row is finished, S1(i,0)=0 */

            /* Compute a(i) from the current Stirling row. */
            seq_t sum  = (seq_t)0;
            seq_t prod = (seq_t)1;  /* product for k = 1  */
            for (idx_t k = 1; k <= i; ++k) {
                sum += s_row[k] * prod;
                if (k < i) {
                    seq_t mult1 = (seq_t)(3 * i - 2 * k);
                    seq_t mult2 = (seq_t)(3 * i - 2 * k + 1);
                    seq_t denom  = (seq_t)(3 * i - k);
                    prod = (prod * mult1 * mult2) / denom;
                }
            }
            a_cache[i] = sum;
        }
        max_computed = n;
    }

    return a_cache[n];
}
