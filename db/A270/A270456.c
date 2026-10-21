/*
 * A270456  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 163", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*((1+n)*(2+(-1)^n*n))
 * Range:     i8 n <= 7, i16 n <= 127, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;
    seq_t sign = (n & 1) ? (seq_t)-1 : (seq_t)1;   /* (-1)^n */
    seq_t term = 2 + sign * N;                    /* 2 + (-1)^n * n */
    seq_t res  = 2 * ((N + 1) * term);             /* 2 * (N+1) * term */
    return res;
}
