/*
 * A270873  a(n) = n^9 + 8*n^8 + 43*n^7 + 159*n^6 + 452*n^5 + 997*n^4 +
 *          1725*n^3 + 2272*n^2 + 1990*n + 21.
 * https://oeis.org/A270873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^9 + 8*n^8 + 43*n^7 + 159*n^6 + 452*n^5 + 997*n^4 +
 *            1725*n^3 + 2272*n^2 + 1990*n + 21
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 10, u64 n <= 137, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;          /* cast index to seq_t for arithmetic */
    seq_t r = 1;                 /* coefficient of n^9 */

    r = r * x + 8;               /* +8*n^8 */
    r = r * x + 43;              /* +43*n^7 */
    r = r * x + 159;             /* +159*n^6 */
    r = r * x + 452;             /* +452*n^5 */
    r = r * x + 997;             /* +997*n^4 */
    r = r * x + 1725;            /* +1725*n^3 */
    r = r * x + 2272;            /* +2272*n^2 */
    r = r * x + 1990;            /* +1990*n   */
    r = r * x + 21;              /* +21       */

    return r;
}
