/*
 * A270272  a(n) = binomial(n+3,n)^3.
 * https://oeis.org/A270272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,n)^3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute binomial(n+3,3) = (n+1)(n+2)(n+3)/6 */
    seq_t x = (seq_t)n + 1;
    seq_t y = (seq_t)n + 2;
    seq_t z = (seq_t)n + 3;

    /* Cancel factor 3 */
    if ((x % 3) == 0)
        x /= 3;
    else if ((y % 3) == 0)
        y /= 3;
    else
        z /= 3;

    /* Cancel factor 2 */
    if ((x % 2) == 0)
        x /= 2;
    else if ((y % 2) == 0)
        y /= 2;
    else
        z /= 2;

    seq_t choose = x * y * z;

    /* Now cube */
    seq_t ans = choose * choose * choose;
    return ans;
}
