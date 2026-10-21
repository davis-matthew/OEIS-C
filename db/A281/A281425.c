/*
 * A281425  a(n) = [q^n] (1 - q)^n / Product_{j=1..n} (1 - q^j).
 * https://oeis.org/A281425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{j=0..n} (-1)^j · binomial(n,j) · p(n−j) * where p(m)
 *            is the number of partitions of m with all parts ≤ n
 * Range:     i8 n <= 6, i16 n <= 14, i32 n <= 29, i64 n <= 61, i128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Returns the n‑th term of sequence A281425.  */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* Allocate space for the partition numbers p[0..n] */
#ifdef __cplusplus
    seq_t *p = new seq_t[n + 1];
#else
    seq_t *p = (seq_t *)calloc(n + 1, sizeof(seq_t));
#endif
    if (!p)   /* out of memory – unlikely to happen */
        return (seq_t)0;

    p[0] = (seq_t)1;

    /* Compute partitions using the generalized pentagonal recurrence. */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; ; ++k) {
            /* Generalized pentagonal numbers. */
            idx_t pent1 = k * (3 * k - 1) / 2;
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent1 > m && pent2 > m)
                break;

            if (pent1 <= m) {
                seq_t term = p[m - pent1];
                if (k & 1)
                    sum += term;
                else
                    sum -= term;
            }

            if (pent2 <= m) {
                seq_t term = p[m - pent2];
                if (k & 1)
                    sum += term;
                else
                    sum -= term;
            }
        }
        p[m] = sum;
    }

    /* Evaluate a(n) directly from the binomial transform. */
    seq_t result = (seq_t)0;
    seq_t binom   = (seq_t)1;   /* C(n,0) */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t term = p[n - j];
        if (j & 1)
            term = -term;
        result += binom * term;

        if (j != n) {
            binom = binom * (seq_t)(n - j);
            binom = binom / (seq_t)(j + 1);
        }
    }

#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif

    return result;
}
