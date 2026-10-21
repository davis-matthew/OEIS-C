/*
 * A296194  Harary index of the n-transposition graph.
 * https://oeis.org/A296194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/2) * Σ_{k=1}^{n-1} |Stirling1(n, n-k)| / k (unsigned
 *            Stirling numbers)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* below offset, return 0 */
        return 0;

    /* Allocate array for Stirling numbers of the first kind (unsigned) up to order n. */
    seq_t *c = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!c)
        return 0;   /* allocation failed */

    /* Initialization: S(0,0)=1, S(0,k>0)=0 */
    for (idx_t i = 0; i <= n; ++i)
        c[i] = 0;
    c[0] = 1;

    /* Build successive rows.
       Recurrence: S(m,k) = S(m-1,k-1) + (m-1)*S(m-1,k)  (unsigned). */
    for (idx_t m = 1; m <= n; ++m) {
        for (idx_t k = m; k >= 1; --k) {
            c[k] = c[k - 1] + (seq_t)(m - 1) * c[k];
        }
        /* After the row, S(m,0)=0 */
        c[0] = 0;
    }

    /* Compute n! */
    seq_t fact = factorial(n);

    /* Sum the required terms: (n!/2) * Σ |S(n, n-k)| / k. */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n - 1; ++k) {
        seq_t denom = (seq_t)2 * (seq_t)k;
        seq_t term = fact * c[n - k];
        term = term / denom;          /* exact division */
        sum += term;
    }

    free(c);
    return sum;
}
