/*
 * A242150  Number of length 6+5 0..n arrays with no consecutive six elements
 *          summing to more than 3*n.
 * https://oeis.org/A242150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1956631/9979200)*n^11 + (1726129/725760)*n^10 +
 *            (9526243/725760)*n^9 * + (5272471/120960)*n^8 +
 *            (19548049/201600)*n^7 + (5255677/34560)*n^6 * +
 *            (124917323/725760)*n^5 + (25617881/181440)*n^4 +
 *            (75074821/907200)*n^3 * + (83833/2520)*n^2 + (115589/13860)*n +
 *            1
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const unsigned int D = 39916800u;           /* common denominator */
    /* Scaled integer coefficients for the polynomial (numerator * (D / denominator)) */
    static const unsigned __int128 scaled[12] = {
        (unsigned __int128)7826524ULL,   /* (1956631/9979200) * n^11 */
        (unsigned __int128)94937095ULL,  /* (1726129/725760)  * n^10 */
        (unsigned __int128)523943365ULL, /* (9526243/725760)  * n^9  */
        (unsigned __int128)1739915430ULL,/* (5272471/120960) * n^8  */
        (unsigned __int128)3870513702ULL,/* (19548049/201600)* n^7  */
        (unsigned __int128)6070306935ULL,/* (5255677/34560) * n^6  */
        (unsigned __int128)6870452765ULL,/* (124917323/725760)* n^5  */
        (unsigned __int128)5635933820ULL,/* (25617881/181440)* n^4  */
        (unsigned __int128)3303292124ULL,/* (75074821/907200)* n^3  */
        (unsigned __int128)1327914720ULL,/* (83833/2520)    * n^2  */
        (unsigned __int128)332896320ULL, /* (115589/13860)  * n   */
        (unsigned __int128)39916800ULL   /* constant term            */
    };

    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 res = scaled[0];

    for (int i = 1; i < 12; ++i)
        res = res * N + scaled[i];

    res = res / D;
    return (seq_t)res;
}
