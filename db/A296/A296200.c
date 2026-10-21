/*
 * A296200  Harary index of the n X n white bishop graph.
 * https://oeis.org/A296200
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (21 - 4*n - 42*n^2 + 16*n^3 + 6*n^4 + 3*(-1)^n*(-7 + 4*n
 *            + 2*n^2)) / 96 with a(2) = 1. For n < 2 a(n) is undefined; this
 *            implementation simply returns 0. The formula is evaluated in
 *            128‑bit signed arithmetic to avoid overflow and intermediate
 *            negative values
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2)
        return 0;

    /* Work in signed 128‑bit integer for safety. */
    __int128 nn  = n;
    __int128 n2  = nn * nn;
    __int128 n3  = n2 * nn;
    __int128 n4  = n3 * nn;

    /* Polynomial part */
    __int128 num = 21
                   - 4 * nn
                   - 42 * n2
                   + 16 * n3
                   + 6 * n4;

    /* Alternating part */
    __int128 alt = 3 * ( -7 + 4 * nn + 2 * n2 );

    /* Add or subtract the alternating part depending on parity of n */
    if ((n & 1) == 0)   /* n even: (-1)^n = +1 */
        num += alt;
    else                /* n odd:  (-1)^n = -1 */
        num -= alt;

    /* Final division (exact division by 96). */
    return (seq_t)(num / 96);
}
