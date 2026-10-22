/*
 * A321245  Non-isomorphic proper colorings of the 4 X 4 grid graph using at
 *          most n colors under rotational and reflectional symmetries.
 * https://oeis.org/A321245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/8) * (n^16 - 24*n^15 + 276*n^14 - 2015*n^13 +
 *            10437*n^12 - 40614*n^11 + 122664*n^10 - 292907*n^9 + 557915*n^8
 *            - 848501*n^7 + 1023195*n^6 - 962156*n^5 + 683997*n^4 -
 *            347485*n^3 + 112831*n^2 - 17614*n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* coefficients of the polynomial inside the parentheses,
       from n^0 up to n^16 */
    static const long long coeff[17] = {
        0,          /* n^0  */
        -17614,     /* n^1  */
        112831,     /* n^2  */
        -347485,    /* n^3  */
        683997,     /* n^4  */
        -962156,    /* n^5  */
        1023195,    /* n^6  */
        -848501,    /* n^7  */
        557915,     /* n^8  */
        -292907,    /* n^9  */
        122664,     /* n^10 */
        -40614,     /* n^11 */
        10437,      /* n^12 */
        -2015,      /* n^13 */
        276,        /* n^14 */
        -24,        /* n^15 */
        1           /* n^16 */
    };

#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
    /* Use 128‑bit intermediate arithmetic when available. */
    __int128 p = coeff[16];
    for (int i = 15; i >= 0; --i) {
        p = p * (__int128)n + coeff[i];
    }
    return (seq_t)(p / 8);
#else
    /* Fallback: assume seq_t has enough capacity (128‑bit or 
       arbitrary‑precision).  This branch is rarely used on common
       compilers that provide __int128. */
    seq_t p = coeff[16];
    for (int i = 15; i >= 0; --i) {
        p = p * (seq_t)n + (seq_t)coeff[i];
    }
    return p / 8;
#endif
}
