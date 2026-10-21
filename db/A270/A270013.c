/*
 * A270013  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 7", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=8; for n>0, a(n)=4+5*(-1)^n+(4+8*(-1)^n)*n+4*(-1)^n*n^2
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)8;

    /* (-1)^n : 1 if n even, -1 if n odd */
    seq_t sign = (n & 1ULL) ? (seq_t)-1 : (seq_t)1;
    seq_t sn   = (seq_t)n;

    seq_t term1 = (seq_t)4 + (seq_t)5 * sign;
    seq_t term2 = ((seq_t)4 + (seq_t)8 * sign) * sn;
    seq_t term3 = (seq_t)4 * sign * sn * sn;

    return term1 + term2 + term3;
}
