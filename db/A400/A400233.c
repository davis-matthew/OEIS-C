/*
 * A400233  a(n) is the number of boundary lattice points minus the number of
 *          interior lattice points of the 4-dimensional cube [0,n]^4.
 * https://oeis.org/A400233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n+1)^4 - 2*(n-1)^4
 * Range:     i8 n <= 2, i16 n <= 17, i32 n <= 218, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Use __int128 for intermediate calculations to avoid signed overflow. */
    __int128 nn = (__int128)n;
    __int128 res = -nn * nn * nn * nn
                   + 12 * nn * nn * nn
                   - 6  * nn * nn
                   + 12 * nn
                   - 1;
    return (seq_t)res;
}
