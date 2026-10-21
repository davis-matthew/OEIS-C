/*
 * A271057  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 259", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=4; a(1)=0; for n≥2, let k=floor(n/2); a(2k)=16k²+24k−9;
 *            a(2k+1)=−a(2k);
 * Range:     i8 n <= 5, i16 n <= 89, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 4;
    if (n == 1) return 0;
    /* k = floor(n/2) */
    seq_t k = n / 2;
    seq_t val = (seq_t)16 * k * k + (seq_t)24 * k - (seq_t)9;
    if (n & 1) return -val;
    return val;
}
