/*
 * A371160  Expansion of e.g.f. 1/(1 - x^3 - x^4)^x.
 * https://oeis.org/A371160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{j=0..floor(n/3)} Sum_{k=0..j}
 *            binomial(j,n-3*j-k) * * |Stirling1(j,k)|/j!
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n! */
    seq_t fact_n = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        fact_n *= (seq_t)i;
    }

    /* max j to consider */
    idx_t Jmax = n / 3;
    /* allocate working arrays */
    seq_t *prev = (seq_t*)calloc(Jmax + 2, sizeof(seq_t));
    seq_t *curr = (seq_t*)calloc(Jmax + 2, sizeof(seq_t));
    seq_t *binom = (seq_t*)calloc(Jmax + 1, sizeof(seq_t));
    if (!prev || !curr || !binom) {
        return (seq_t)0;   /* allocation failure – return zero */
    }

    prev[0] = (seq_t)1;          /* Stirling numbers for j = 0: s(0,0)=1 */
    seq_t ratio = fact_n;        /* will hold n! / j! for current j */
    seq_t sum   = (seq_t)0;      /* accumulator for a(n) */

    for (idx_t j = 0; j <= Jmax; ++j) {
        if (j > 0) {
            ratio /= (seq_t)j;   /* divide by j to get n! / j! */
        }

        /* compute binomial row for current j: C(j,m) for 0 <= m <= j */
        binom[0] = (seq_t)1;
        for (idx_t m = 1; m <= j; ++m) {
            binom[m] = binom[m - 1] * (seq_t)(j - m + 1) / (seq_t)m;
        }

        idx_t tmp = n - 3 * j;          /* n - 3j  */
        for (idx_t k = 0; k <= j; ++k) {
            if (k > tmp) break;         /* n - 3j - k < 0 → stop */
            idx_t m = tmp - k;          /* m = n - 3j - k */
            if (m > j) continue;        /* m must be <= j */
            seq_t term = binom[m] * prev[k];
            term *= ratio;
            sum += term;
        }

        /* prepare Stirling numbers for next j (if any) */
        if (j < Jmax) {
            curr[0] = (seq_t)0;
            for (idx_t k = 1; k <= j + 1; ++k) {
                seq_t val1 = prev[k - 1];
                seq_t val2 = (k <= j) ? prev[k] : (seq_t)0;
                curr[k] = val1 + (seq_t)j * val2;
            }
            /* swap prev and curr */
            seq_t *tmp_ptr = prev;
            prev = curr;
            curr = tmp_ptr;
        }
    }

    /* free allocated memory */
    free(prev);
    free(curr);
    free(binom);

    return sum;
}
