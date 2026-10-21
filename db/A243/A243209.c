/*
 * A243209  Number of inequivalent (mod D_3) ways to place 4 points on a
 *          triangular grid of side n so that they are not vertices of an
 *          equilateral triangle with sides parallel to the grid.
 * https://oeis.org/A243209
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 + 4*n^7 - 6*n^6 - 80*n^5 + 60*n^4 + 208*n^3 +
 *            464*n^2 - 1152*n)/2304 + IF(n%2==1) * (28*n^3 - 206*n^2 + 312*n
 *            + 33)/768 + IF(n%3==1) * (n^2 - 2*n + 4)/18 + IF(n%6==1) *
 *            (-1)/6 (All fractions are integer-valued; the final sum is an
 *            integer.) Computed via a common denominator 2304 for s...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 41, u64 n <= 673,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* offset 3, below undefined range */

    __int128 nn = (unsigned __int128)n;   /* cast to 128 bit to avoid overflow */
    __int128 n2 = nn * nn;                /* n^2   */
    __int128 n3 = n2 * nn;                /* n^3   */
    __int128 n4 = n3 * nn;                /* n^4   */
    __int128 n5 = n4 * nn;                /* n^5   */
    __int128 n6 = n5 * nn;                /* n^6   */
    __int128 n7 = n6 * nn;                /* n^7   */
    __int128 n8 = n7 * nn;                /* n^8   */

    /* Base term with denominator 2304 */
    __int128 numerator =
        n8 + 4 * n7 - 6 * n6 - 80 * n5 + 60 * n4 + 208 * n3 + 464 * n2 - 1152 * nn;

    /* 2‑modular correction   (denominator 768; multiply numerator by 3) */
    if (n & 1ULL) {
        __int128 term = (28 * n3 - 206 * n2 + 312 * nn + 33) * 3;
        numerator += term;
    }

    /* 3‑modular correction   (denominator 18; multiply numerator by 128) */
    if (n % 3ULL == 1ULL) {
        __int128 term = (n2 - 2 * nn + 4) * 128;
        numerator += term;
    }

    /* 6‑modular correction   (denominator 6; multiply numerator by 384) */
    if (n % 6ULL == 1ULL) {
        numerator -= 384;
    }

    /* All terms share denominator 2304; final division gives integer result */
    __int128 result = numerator / 2304;
    return (seq_t)result;
}
