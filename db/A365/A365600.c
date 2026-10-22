/*
 * A365600  Expansion of e.g.f. 1 / (1 - 4 * log(1 + x))^(3/4).
 * https://oeis.org/A365600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (Product_{j=0..k-1} (4*j+3)) *
 *            |Stirling1(n,k)| where |Stirling1(n,k)| are the unsigned
 *            Stirling numbers of the first kind. The sequence is calculated
 *            using only non‑negative integer operations
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* allocate arrays for unsigned Stirling numbers and products */
    seq_t *c = (seq_t *)calloc(n + 1, sizeof(seq_t));     /* c[k] = |s(n,k)| for current n */
    seq_t *p = (seq_t *)calloc(n + 1, sizeof(seq_t));     /* p[k] = product_{j=0..k-1} (4*j+3) */

    if (!c || !p) return (seq_t)0;             /* allocation failure */

    /* initial values: for n = 0  (c[0] = 1 already) */
    c[0] = (seq_t)1;
    p[0] = (seq_t)1;

    /* iterative construction */
    for (idx_t m = 1; m <= n; ++m) {
        /* update product array */
        p[m] = p[m - 1] * ((seq_t)(4 * (m - 1) + 3));

        /* update unsigned Stirling numbers of the first kind in place
           using recurrence   c[n][k] = c[n-1][k-1] + (n-1)*c[n-1][k]   */
        for (idx_t k = m; k >= 1; --k) {
            /* old value of c[k] is still present because we are iterating downward */
            c[k] = c[k - 1] + (seq_t)(m - 1) * c[k];
        }
        /* c[0] becomes 0 for the next iteration */
        c[0] = (seq_t)0;
        /* the last element of the row must be 1 */
        c[m] = (seq_t)1;

        /* accumulation of even/odd parity sums for current n = m */
        seq_t sumEven = (seq_t)0;   /* sum over even k */
        seq_t sumOdd  = (seq_t)0;   /* sum over odd  k */

        for (idx_t k = 0; k <= m; ++k) {
            /* product * |s| */
            seq_t term = p[k] * c[k];
            if ((k & 1) == 0)              /* even k */
                sumEven += term;
            else                           /* odd k */
                sumOdd  += term;
        }

        /* store a(n) if we reach the requested n */
        if (m == n) {
            seq_t result;
            if ((n & 1) == 0)   /* n even: a = even - odd */
                result = sumEven - sumOdd;
            else                /* n odd:  a = odd  - even */
                result = sumOdd  - sumEven;

            /* free memory */
            free(c);
            free(p);
            return result;
        }
    }

    /* should not reach here */
    free(c);
    free(p);
    return (seq_t)0;
}
