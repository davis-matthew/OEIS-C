/*
 * A290363  Number of 9-leaf rooted trees with n levels.
 * https://oeis.org/A290363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1385*n^8 +1124*n^7 +4018*n^6 +6776*n^5 +7945*n^4
 *            +9716*n^3 +6812*n^2 +2544*n)/8!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 24, u64 n <= 390, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: 8! = 40320.  The calculation is performed with 128‑bit intermediates to avoid overflow. */
seq_t a(idx_t n)
{
    /* Use 128‑bit integer for intermediate computation */
    __int128 n128 = (__int128)n;
    __int128 power = 1;          /* n^0 initially */
    __int128 num   = 0;

    /* n^1 */
    power *= n128;
    num += 2544 * power;         /* 2544*n^1 */

    /* n^2 */
    power *= n128;
    num += 6812 * power;         /* 6812*n^2 */

    /* n^3 */
    power *= n128;
    num += 9716 * power;         /* 9716*n^3 */

    /* n^4 */
    power *= n128;
    num += 7945 * power;         /* 7945*n^4 */

    /* n^5 */
    power *= n128;
    num += 6776 * power;         /* 6776*n^5 */

    /* n^6 */
    power *= n128;
    num += 4018 * power;         /* 4018*n^6 */

    /* n^7 */
    power *= n128;
    num += 1124 * power;         /* 1124*n^7 */

    /* n^8 */
    power *= n128;
    num += 1385 * power;         /* 1385*n^8 */

    /* Divide by 40320 to obtain the integer result */
    seq_t result = (seq_t)(num / 40320);
    return result;
}
