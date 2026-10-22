/*
 * A352680  Array read by ascending antidiagonals. A family of Catalan-like
 *          sequences. A(n, k) = [x^k] ((n - 1)*x + 1)*(1 - sqrt(1 -
 *          4*x))/(2*x).
 * https://oeis.org/A352680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) enumerates the array A(n,k) by ascending antidiagonals.
 *            A(n,0) = 1 For k≥1: A(n,k) = (n-1)*Catalan(k-1) + Catalan(k) Let
 *            d = ⌊(√(1+8*n) – 1)/2⌋ be the diagonal index. offset = n –
 *            d*(d+1)/2 i = d – offset (row index) j = offset (column index)
 *            Then a(n) = A(i,j)
 * Range:     u8 n <= 26, u16 n <= 64, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 <= n)
        d++;

    idx_t start  = d * (d + 1) / 2;   /* first index on this diagonal   */
    idx_t offset = n - start;        /* 0 <= offset <= d              */
    idx_t i = d - offset;            /* row index                      */
    idx_t j = offset;                /* column index                   */

    /* A(i,0) is always 1. */
    if (j == 0)
        return (seq_t)1;

    seq_t cj  = catalan(j);          /* Catalan(k)                      */
    seq_t cj1 = catalan(j - 1);      /* Catalan(k-1)                    */

    /* Handle i==0 separately to avoid unsigned underflow. */
    if (i == 0)
        return cj - cj1;              /* equals Catalan(j) - Catalan(j-1) */

    /* General case: (i-1)*Catalan(j-1) + Catalan(j). */
    seq_t prod = (seq_t)(i - 1) * cj1;
    return prod + cj;
}
