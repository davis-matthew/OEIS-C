/*
 * A296917  List of coefficients of chromatic polynomial of icosahedron,
 *          highest order terms first.
 * https://oeis.org/A296917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of k^(13-n) in the chromatic polynomial of
 *            the icosahedron: k^12 - 30*k^11 + 415*k^10 - 3500*k^9 +
 *            20023*k^8 - 81622*k^7 + 241605*k^6 - 517360*k^5 + 780286*k^4 -
 *            782108*k^3 + 463310*k^2 - 121020*k + 0. Thus a(1)=1, a(2)=-30,
 *            …, a(13)=0; all further terms are zero
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 13, i64 n <= 13, i128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* coefficients are ordered from k^12 to k^0 */
    static const seq_t coeffs[13] = {
        (seq_t)1,        /* k^12 */
        (seq_t)-30,      /* k^11 */
        (seq_t)415,      /* k^10 */
        (seq_t)-3500,    /* k^9  */
        (seq_t)20023,    /* k^8  */
        (seq_t)-81622,   /* k^7  */
        (seq_t)241605,   /* k^6  */
        (seq_t)-517360,  /* k^5  */
        (seq_t)780286,   /* k^4  */
        (seq_t)-782108,  /* k^3  */
        (seq_t)463310,   /* k^2  */
        (seq_t)-121020,  /* k^1  */
        (seq_t)0         /* k^0  */
    };
    if (n >= 1 && n <= 13)
        return coeffs[n - 1];
    return (seq_t)0;
}
