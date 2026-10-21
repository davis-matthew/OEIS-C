/*
 * A271256  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 323", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n^2+4*n+3 for n>4 and even; a(n) = -4*n^2+4*n+1 for n>4
 *            and odd; a(0) = 4, a(1) = 0, a(2) = 31, a(3) = -27, a(4) = 87
 * Range:     i8 n <= 5, i16 n <= 91, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle initial values */
    switch (n) {
        case 0: return (seq_t)4;
        case 1: return (seq_t)0;
        case 2: return (seq_t)31;
        case 3: return (seq_t)-27;
        case 4: return (seq_t)87;
    }

    /* for n > 4 use the explicit formula */
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;

    if ((n & 1) == 0) {
        /* even n */
        return (seq_t)4 * sq + (seq_t)4 * sn + (seq_t)3;
    } else {
        /* odd n */
        return -( (seq_t)4 * sq ) + (seq_t)4 * sn + (seq_t)1;
    }
}
