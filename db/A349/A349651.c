/*
 * A349651  E.g.f. satisfies: A(x)^(A(x)^3) = 1 + x.
 * https://oeis.org/A349651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n-1) * Sum_{k=0..n} (3*k-1)^(k-1) *
 *            |Stirling1(n,k)|
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 12, i128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Prepare two rows for Stirling numbers of the first kind (unsigned). */
    const size_t size = (size_t)n + 1;
#ifdef __cplusplus
    seq_t *row   = new seq_t[size];
    seq_t *row_n = new seq_t[size];
#else
    seq_t *row   = (seq_t*)calloc(size, sizeof(seq_t));
    seq_t *row_n = (seq_t*)calloc(size, sizeof(seq_t));
#endif

    /* Initialise S(0,0) = 1. */
    row[0] = (seq_t)1;

    /* Build Stirling numbers up to n. */
    for (idx_t i = 1; i <= n; ++i) {
        row_n[0] = (seq_t)0;                     /* S(i,0) = 0 */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = row[k - 1];              /* S(i-1, k-1) */
            if (k <= i - 1) {
                term += (seq_t)(i - 1) * row[k];  /* (i-1)*S(i-1,k) */
            }
            row_n[k] = term;                      /* S(i,k) */
        }
        /* Swap rows for next iteration. */
        seq_t *tmp = row;
        row = row_n;
        row_n = tmp;
    }

    /* Compute the weighted sum for the final row. */
    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base  = (seq_t)(3 * k - 1);
        seq_t exp   = (seq_t)(k - 1);
        seq_t w     = big_pow(base, exp);       /* (3*k-1)^(k-1) */
        sum += w * row[k];                      /* add weighted Stirling number */
    }

    /* Apply the overall sign (-1)^(n-1). */
    if ((n - 1) & (idx_t)1) {
        sum = -sum;
    }

    /* Clean up allocated memory. */
#ifdef __cplusplus
    delete[] row;
    delete[] row_n;
#else
    free(row);
    free(row_n);
#endif

    return sum;
}
