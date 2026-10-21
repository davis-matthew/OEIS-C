/*
 * A291977  Triangle read by rows, T(n, k) = Sum_{j=0..n}
 *          (-1)^(k-j)*Eulerian1(n, j)* binomial(n-j, n-k) for 0 <= k <= n.
 * https://oeis.org/A291977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r and k are obtained from the flattened
 *            triangle index n: r = floor((sqrt(8*n+1)-1)/2) k = n - r*(r+1)/2
 *            T(r,k) = sum_{j=0..k} (-1)^{k-j} * Eulerian(r,j) * binomial(r-j,
 *            k-j)
 * Range:     i8 n <= 23, i16 n <= 48, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: Uses the utility function binomial(idx_t a, idx_t b) */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /*----------- static cache -------------------------------------*/
    static seq_t *vals = NULL;        /* flattened triangle values   */
    static size_t  val_cap = 0;       /* capacity of vals           */
    static size_t  val_len = 0;       /* number of values stored    */

    static seq_t *e_prev = NULL;      /* Eulerian numbers of last row */
    static idx_t  e_prev_n = 0;       /* row number of e_prev         */

    /*----------- one‑time initialization ---------------------------*/
    if (vals == NULL) {
        /* start with row 0 */
        val_cap = 8;
        vals = (seq_t*)malloc(val_cap * sizeof(seq_t));
        vals[0] = (seq_t)1;
        val_len = 1;

        e_prev = (seq_t*)malloc(1 * sizeof(seq_t));
        e_prev[0] = (seq_t)1;
        e_prev_n = 0;
    }

    /*----------- extend cache until we cover index n ---------------*/
    while (val_len <= n) {
        idx_t r = e_prev_n + 1;                     /* new row number   */
        idx_t r_plus = r + 1;                       /* used for binom   */

        /* compute Eulerian numbers for row r */
        seq_t *e_curr = (seq_t*)malloc((r_plus) * sizeof(seq_t));
        /* element 0 is always 1 */
        e_curr[0] = (seq_t)1;
        for (idx_t k = 1; k < r; ++k) {
            seq_t term1 = (seq_t)(k + 1) * e_prev[k];
            seq_t term2 = (seq_t)(r - k) * e_prev[k - 1];
            e_curr[k] = term1 + term2;
        }
        e_curr[r] = (seq_t)0;           /* Eulerian(r,r) = 0 */

        /* compute triangle values for this row (k = 0..r) */
        for (idx_t k = 0; k <= r; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j <= k; ++j) {
                int diff = (int)(k - j);
                seq_t sign = (diff & 1) ? (seq_t)-1 : (seq_t)1;

                /* binomial(r-j, k-j) is valid because k-j <= r-j */
                seq_t bin = binomial(r - j, k - j);
                seq_t term = e_curr[j] * bin;
                sum = (sign < 0) ? (sum - term) : (sum + term);
            }
            if (val_len == val_cap) {
                val_cap <<= 1;
                vals = (seq_t*)realloc(vals, val_cap * sizeof(seq_t));
            }
            vals[val_len++] = sum;
        }

        /* free previous Eulerian array and remember the current one */
        free(e_prev);
        e_prev = e_curr;
        e_prev_n = r;
    }

    return vals[n];
}
