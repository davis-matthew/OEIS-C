/*
 * A292537  Number of cliques in the n-Sierpinski tetrahedron graph.
 * https://oeis.org/A292537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*4^n/2 + 3 (for n>1), a(1)=16
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS A292537.
   For n >= 1:
     a(1) = 16
     a(n) = 7*2^(2n-1) + 3  for n > 1
*/
seq_t a(idx_t n) {
    if (n == 0)
        return 0;                /* undefined for offset 1, but safe for n=0 */

    if (n == 1)
        return (seq_t)16;

    /* Compute 2^(2n-1) by repeated doubling. */
    idx_t shift = 2 * n - 1;
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < shift; ++i)
        pow2 *= (seq_t)2;

    return (seq_t)7 * pow2 + (seq_t)3;
}
