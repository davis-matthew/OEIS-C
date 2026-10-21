/*
 * A278925  a(n) = permanent M_n where M_n is the n X n matrix m(i,j) = i^3 +
 *          j^3.
 * https://oeis.org/A278925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = permanent of an n × n matrix with entries m(i,j) = i³ +
 *            j³ (1 ≤ i,j ≤ n); a(0)=1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate arrays for elementary symmetric sums e[0..n]
       and factorials fac[0..n]. */
    seq_t *e   = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *fac = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!e || !fac) return 0;          /* failure – unlikely in tests */

    /* Initialize e[0] = 1, others 0 */
    e[0] = 1;
    for (idx_t k = 1; k <= n; ++k) e[k] = 0;

    /* Build elementary symmetric sums of i³ for i = 1..n */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = (seq_t)i * (seq_t)i * (seq_t)i;   /* i³ */
        idx_t k = i;
        while (1) {
            /* e[k] += e[k-1] * i³ */
            e[(size_t)k] = e[(size_t)k] + e[(size_t)k - 1] * val;
            if (k == 1) break;
            --k;
        }
    }

    /* Compute factorials fac[i] = i!  */
    fac[0] = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fac[(size_t)i] = fac[(size_t)i - 1] * (seq_t)i;
    }

    /* permanent = Σ_{k=0}^n  k! (n–k)! e_k e_{n–k} */
    seq_t perm = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t temp = e[(size_t)k] * e[(size_t)(n - k)];
        temp = temp * fac[(size_t)k];
        temp = temp * fac[(size_t)(n - k)];
        perm = perm + temp;
    }

    /* Clean up and return */
    free(e);
    free(fac);
    return perm;
}
