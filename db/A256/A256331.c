/*
 * A256331  Number of Largest Hairpin Family matchings on n edges.
 * https://oeis.org/A256331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2 * sum_{i=1}^{n-1} a[i] * a[n-i] - S3(n-1)
 *            where S3(m) = sum_{i=0}^{m} a[i] * sum_{j=0}^{m-i} a[j] *
 *            a[m-i-j] Sequence A256331 (offset 1): a(1)=1, a(2)=3, a(3)=14,
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;   /* not part of the offset but harmless */
    }

    size_t sz = (size_t)n + 1;          /* store a[0..n] */
    seq_t *a = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *s2 = (seq_t *)calloc(sz, sizeof(seq_t));

    a[0] = (seq_t)1;                   /* a(0) = 1 for the generating function */
    s2[0] = (seq_t)1;                  /* s2[0] = a0 * a0 */

    /* Main loop computing a(1) .. a(n) */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t S2_nm1 = s2[(size_t)(m - 1)];   /* S2(m-1) */
        seq_t T = s2[(size_t)m];             /* T = sum_{i=1}^{m-1} a[i]*a[m-i]  (old S2[m]) */

        /* Compute S3(m-1) = sum_{i=0}^{m-1} a[i] * S2(m-1-i) */
        seq_t S3_nm1 = (seq_t)0;
        for (idx_t i = 0; i < m; ++i) {
            S3_nm1 += a[(size_t)i] * s2[(size_t)(m - 1 - i)];
        }

        /* Use recurrence: a[m] = 2*S2_nm1 + 2*T - S3_nm1 */
        seq_t temp = (seq_t)2 * S2_nm1 + (seq_t)2 * T;
        a[(size_t)m] = temp - S3_nm1;

        /* Update s2[k] for k from m to min(2*m, n) to include new a[m] */
        idx_t upper = 2 * m;
        if (upper > n) upper = n;
        for (idx_t k = m; k <= upper; ++k) {
            if (k == 2 * m) {
                /* k == 2*m: pair (m,m) counted once */
                s2[(size_t)k] += a[(size_t)m] * a[(size_t)m];
            } else {
                /* pairs (m, k-m) and (k-m, m) are distinct for k < 2*m */
                s2[(size_t)k] += (seq_t)2 * a[(size_t)m] * a[(size_t)(k - m)];
            }
        }
    }

    seq_t result = a[(size_t)n];
    free(a);
    free(s2);
    return result;
}
