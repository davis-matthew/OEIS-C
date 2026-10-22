/*
 * A346543  a(n) = [x^n] Product_{k=1..2*n} (x + (2*k-1)^2).
 * https://oeis.org/A346543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Product_{k=1..2n} (x + (2*k-1)^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* allocate array for elementary symmetric sums up to degree n */
    size_t size = (size_t)n + 1;
    seq_t *e = (seq_t *)calloc(size, sizeof(seq_t));
    if (!e) return (seq_t)0;      /* memory failure, return zero */

    e[0] = (seq_t)1;                /* e_0 = 1 */

    /* process the 2n linear factors */
    for (idx_t k = 1; k <= 2 * n; ++k) {
        seq_t val = (seq_t)(2 * k - 1) * (seq_t)(2 * k - 1); /* (2k-1)^2 */
        idx_t upper = (k < n) ? k : n;                       /* max degree needed */
        for (idx_t j = upper; j >= 1; --j) {
            /* e[j] = e[j] + e[j-1] * val */
            e[j] = e[j] + e[j - 1] * val;
        }
    }

    seq_t result = e[n];
    free(e);
    return result;
}
