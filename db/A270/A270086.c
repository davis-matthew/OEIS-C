/*
 * A270086  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 65", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2+(-1)^n+4*n+4*(-1)^n*n^2 for n>4 Initial values: a(0)=3,
 *            a(1)=1, a(2)=31, a(3)=-27, a(4)=87
 * Range:     i8 n <= 5, i16 n <= 91, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle the first five terms explicitly */
    switch (n) {
        case 0: return 3;
        case 1: return 1;
        case 2: return 31;
        case 3: return -27;
        case 4: return 87;
    }
    /* n >= 5 */
    seq_t nn = (seq_t)n;
    if ((n & 1u) == 0) { /* even n */
        return 4 * nn * nn + 4 * nn + 3;
    } else {               /* odd n */
        return -4 * nn * nn + 4 * nn + 1;
    }
}
