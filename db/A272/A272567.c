/*
 * A272567  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 501", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=7, a(1)=-3, a(2)=39, a(3)=-31, a(4)=99, a(5)=-87; for n>5:
 *            if n even a(n)=4*n^2+4*n+15 else a(n)=-4*n^2+4*n-3
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first six terms explicitly */
    if (n == 0) return (seq_t)  7;
    if (n == 1) return (seq_t) -3;
    if (n == 2) return (seq_t) 39;
    if (n == 3) return (seq_t) -31;
    if (n == 4) return (seq_t) 99;
    if (n == 5) return (seq_t) -87;

    /* For n > 5 use the closed‑form conjecture */
    const seq_t n_seq = (seq_t)n;
    const seq_t n_sq  = n_seq * n_seq;        /* n^2, fits in seq_t for n <= 1e4 */

    if ((n & 1) == 0)                         /* even n */
        return 4 * n_sq + 4 * n_seq + (seq_t)15;
    else                                      /* odd n  */
        return -4 * n_sq + 4 * n_seq - (seq_t)3;
}
