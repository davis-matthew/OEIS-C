/*
 * A331327  T(n, k) = [x^k] Pochhammer(x, n-k) for n >= 0, 0 <= k <=
 *          floor(n/2). Irregular triangle read by rows.
 * https://oeis.org/A331327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 36, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A331327:  a(n) is the coefficient of x^k in the rising factorial
 * (x)_{n-k} (unsigned Stirling number of the first kind) read by rows.
 * Row r has floor(r/2)+1 terms for k=0..floor(r/2); the linear index n
 * runs over the rows in order.  The function computes a(n) using
 * the recurrence for unsigned Stirling numbers of the first kind:
 *   c(0,0)=1,  c(m,0)=0 (m>0),  c(m,m)=1,
 *   c(m,k)=c(m-1,k-1)+(m-1)*c(m-1,k).  */
/* The implementation builds a memoised 2‑D table up to the row needed. */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Find the row r and column k corresponding to the linear index n */
    idx_t total = 0;
    idx_t r = 0;
    while (1) {
        idx_t len = (r >> 1) + 1;        /* floor(r/2)+1 terms in this row */
        if (total + len > n) break;
        total += len;
        r++;
    }
    idx_t k = n - total;                /* column within row r */

    /* What we really need is T(r, k) = c(r-k, k) */
    idx_t n0 = r - k;   /* argument of Stirling function */
    idx_t k0 = k;

/*--- Memoised Stirling numbers table --------------------------------*/
    static seq_t **S = NULL;
    static idx_t max_n = 0;              /* largest n0 computed so far */

    /* Initialise on first use */
    if (S == NULL) {
        S = (seq_t **)malloc(sizeof(seq_t *));
        S[0] = (seq_t *)calloc(1, sizeof(seq_t));   /* c(0,0)=1 */
        S[0][0] = (seq_t)1;
    }

    /* Grow the table to reach n0 if required */
    if (n0 > max_n) {
        size_t new_len = n0 + 1;
        S = (seq_t **)realloc(S, new_len * sizeof(seq_t *));
        for (idx_t i = max_n + 1; i <= n0; i++) {
            /* row i has i+1 entries */
            S[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));
            /* c(i,i) = 1 */
            S[i][i] = (seq_t)1;
            /* c(i,0) is already 0 due to calloc */
            /* Compute the remaining entries */
            for (idx_t j = 1; j <= i - 1; j++) {
                seq_t val = S[i - 1][j - 1] + (seq_t)(i - 1) * S[i - 1][j];
                S[i][j] = val;
            }
        }
        max_n = n0;
    }
/*--- End of table handling ------------------------------------------*/

    return S[n0][k0];
}
