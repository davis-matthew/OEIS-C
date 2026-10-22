/*
 * A397423  a(n) = Sum_{j=0..n} abs(Stirling1(n, j)) * binomial(n + j, n) *
 *          n^j.
 * https://oeis.org/A397423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..n} |Stirling1(n, j)| * binomial(n + j, n) * n^j
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *c_vals = NULL;   /* unsigned Stirling numbers of the first kind for current n */
static seq_t *a_vals = NULL;   /* a(n) values for all computed n */
static idx_t  max_n  = (idx_t)-1;  /* highest n for which a_vals has been computed */

seq_t a(idx_t n)
{
    /* initialisation for n = 0 */
    if (max_n == (idx_t)-1) {
        max_n = 0;
        c_vals = (seq_t*)malloc(sizeof(seq_t)*(max_n+1));
        a_vals = (seq_t*)malloc(sizeof(seq_t)*(max_n+1));
        if (!c_vals || !a_vals) return (seq_t)0;   /* allocation failed */
        c_vals[0] = (seq_t)1;      /* |S(0,0)| = 1 */
        a_vals[0] = (seq_t)1;      /* a(0) = 1 */
    }

    /* extend computation up to the requested n */
    if (n > max_n) {
        idx_t prev_max;
        for (idx_t m = max_n + 1; m <= n; ++m) {
            prev_max = m - 1;

            /* allocate new array for |S(m, j)| (size m+1) */
            seq_t *new_c = (seq_t*)malloc(sizeof(seq_t)*(m+1));
            if (!new_c) return (seq_t)0;   /* allocation failed */

            /* compute new Stirling numbers and a(m) in one pass */
            new_c[0] = (seq_t)0;          /* |S(m,0)| = 0 for m>0 */
            seq_t sum = (seq_t)0;         /* a(m) will be accumulated here */

            /* combinatorial helpers for A_j = binomial(m + j, j) * m^j */
            seq_t binom = (seq_t)1;       /* binomial(m + 0, 0) = 1 */
            seq_t pow_m = (seq_t)1;       /* m^0 = 1 */

            for (idx_t j = 1; j <= m; ++j) {
                /* compute |S(m, j)| using |S(m-1, j-1)| + (m-1)|S(m-1, j)| */
                seq_t prev_jm1 = c_vals[j-1];
                seq_t prev_j   = (j <= prev_max) ? c_vals[j] : (seq_t)0;
                seq_t s_val = prev_jm1 + ((seq_t)(prev_max)) * prev_j;
                new_c[j] = s_val;

                /* update combinatorial helpers to get A_j for current j */
                binom = binom * ((seq_t)(m + j)) / ((seq_t)j);
                pow_m = pow_m * ((seq_t)m);
                seq_t A_j = binom * pow_m;

                /* accumulate contribution to a(m) */
                sum += s_val * A_j;
            }

            /* a(m) = sum (no need to add contribution from j = 0 as it is 0) */
            /* free old Stirling array and adopt the new one */
            free(c_vals);
            c_vals = new_c;

            /* enlarge a_vals array to store a(m) */
            a_vals = (seq_t*)realloc(a_vals, sizeof(seq_t)*(m+1));
            if (!a_vals) return (seq_t)0;   /* allocation failed */

            a_vals[m] = sum;
            max_n = m;
        }
    }

    /* return the already computed value */
    return a_vals[n];
}
