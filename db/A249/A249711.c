/*
 * A249711  Number of length 5+3 0..n arrays with every four consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A249711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5*n^5 + 30*n^4 + 48*n^3 + 24*n^2 + 4*n)/3 + 1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 63, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Return the n-th term of OEIS A249711.  */
seq_t a(idx_t n)
{
    /* Use the type seq_t for all arithmetic so the function works with
       any integer width or an arbitrary‑precision integer type.      */
    seq_t N  = n;      /* n as seq_t */
    seq_t n2 = N * N;  /* n^2 */
    seq_t n3 = n2 * N; /* n^3 */
    seq_t n4 = n3 * N; /* n^4 */
    seq_t n5 = n4 * N; /* n^5 */

    /* numerator = 5*n^5 + 30*n^4 + 48*n^3 + 24*n^2 + 4*n   */
    seq_t numerator = (5 * n5) + (30 * n4) + (48 * n3) + (24 * n2) + (4 * N);

    /* The numerator is always divisible by 3.  */
    return numerator / 3 + 1;
}
