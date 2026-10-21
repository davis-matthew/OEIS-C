/*
 * A243238  Table T(n,r) of terms in the reverse and add sequences of positive
 *          integers n read by antidiagonals.
 * https://oeis.org/A243238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = the element on the n‑th antidiagonal of the
 *            reverse-and-add table. The mapping is: for the smallest integer
 *            d such that d(d+1)/2 ≥ n, let i = n – (d-1)d/2 and j = d – i +
 *            1. Then a(n) is the j‑th term (1‑indexed) of the reverse-and-add
 *            sequence starting from the integer i, i.e. T(i,j) wher...
 * Range:     u8 n <= 28, u16 n <= 91, u32 n <= 302, u64 n <= 1035,
 *            u128 n <= 4007, big any n
 * Helpers:   reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/reverse_decimal.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* safety for n < 1 */

    /* find the diagonal d with d(d+1)/2 ≥ n */
    idx_t d = 1;
    idx_t triangular = 1;       /* d*(d+1)/2 */
    while (triangular < n) {
        ++d;
        triangular = d * (d + 1) / 2;
    }

    idx_t prev_triangular = (d - 1) * d / 2;
    idx_t i = n - prev_triangular;          /* row (starting integer) */
    idx_t j = d - i + 1;                    /* step index in reverse‑add */

    seq_t x = (seq_t)i;                     /* start value of the sequence */

    if (j == 1) return x;                   /* first term is the number itself */

    for (idx_t step = 2; step <= j; ++step) {
        seq_t rev = reverse_decimal(x);
        x = x + rev;
    }

    return x;
}
