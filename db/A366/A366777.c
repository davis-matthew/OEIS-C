/*
 * A366777  Expansion of e.g.f. -log(1 + x^3 * log(1 - x)).
 * https://oeis.org/A366777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..floor(n/4)} (k-1)! * |Stirling1(n-3*k,
 *            k)|/(n-3*k)!
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for all n >= 0 (with a(0)=0).  The function is
   memoisation‑free: each call is independent.  It uses O(n²/4) time
   and O(n + n/4) memory, acceptable for n up to ~10000. */
seq_t a(idx_t n)
{
    if (n < 4) return 0;                      /* offset 0 */

    /* kmax = floor(n/4) */
    idx_t kmax = n / 4;
    size_t ksize = (size_t)kmax + 1;           /* arrays are 0..kmax */

    /* factorials of 0 .. kmax (small ones) */
    seq_t *fact_small = (seq_t *)malloc(ksize * sizeof(seq_t));
    if (!fact_small) return 0;
    fact_small[0] = 1;
    for (idx_t i = 1; i <= kmax; ++i)
        fact_small[i] = fact_small[i - 1] * (seq_t)i;

    /* suffix products: suff[m] = m! / n!  (i.e. product of (m+1)..n) */
    seq_t *suff = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!suff) { free(fact_small); return 0; }
    suff[n] = 1;
    for (idx_t m = n; m-- > 0; )          /* descending from n-1 to 0 */
        suff[m] = suff[m + 1] * (seq_t)(m + 1);

    /* arrays for Stirling-1 (unsigned) numbers.
       cur holds the row for current m, next is the new row. */
    seq_t *cur = (seq_t *)calloc(ksize, sizeof(seq_t));
    seq_t *next = (seq_t *)malloc(ksize * sizeof(seq_t));
    if (!cur || !next) {
        free(fact_small); free(suff);
        free(cur); free(next);
        return 0;
    }
    cur[0] = 1;                    /* Stirling(0,0) = 1 */

    seq_t result = 0;

    for (idx_t m = 1; m <= n; ++m) {
        next[0] = 0;
        seq_t mult = (seq_t)(m - 1);          /* (m-1) * cur[k] term */
        for (idx_t k = 1; k < ksize; ++k)
            next[k] = cur[k - 1] + mult * cur[k];

        /* swap cur and next */
        seq_t *tmp = cur; cur = next; next = tmp;

        /* check whether current m corresponds to some k in the sum */
        if (m <= n - 3) {                    /* ensures k >= 1 */
            idx_t diff = n - m;
            if (diff % 3 == 0) {
                idx_t k = diff / 3;
                if (k >= 1 && k <= kmax) {
                    seq_t stir = cur[k];
                    seq_t term = suff[m] * fact_small[k - 1];
                    term = term * stir;
                    result += term;
                }
            }
        }
    }

    free(fact_small);
    free(suff);
    free(cur);
    free(next);
    return result;
}
