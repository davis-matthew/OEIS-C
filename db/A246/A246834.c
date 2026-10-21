/*
 * A246834  A(n,k) is the concatenation of n and k*n in binary; square array
 *          A(n,k), n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A246834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base, look
 * Formula:   a(n) = A(i,j) where (i,j) is the n‑th element in the *
 *            antidiagonal traversal of the square array with * A(i,j) =
 *            concat_binary(i, i*j) (concatenation of binary strings)
 * Range:     u8 n <= 62, u16 n <= 2111, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* find diagonal s such that the n‑th element lies on it */
    idx_t s   = 0;
    idx_t cum = 0;                  /* number of elements before diagonal s */

    while (cum + (s + 1) <= n) {
        cum += (s + 1);
        s++;
    }

    /* coordinates (i,j) within the diagonal s */
    idx_t i = n - cum;   /* row index */
    idx_t j = s - i;     /* column index */

    /* compute the concatenation of i and i*j */
    seq_t prod = (seq_t)i * (seq_t)j;   /* i*j */

    if (prod == 0)           /* concatenation with 0 yields i itself */
        return (seq_t)i;

    unsigned shift = bitlen_seq_t(prod);   /* number of bits in i*j */
    return ((seq_t)i << shift) | prod;
}
