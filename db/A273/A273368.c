/*
 * A273368  Numbers k such that 10*k+9 is a perfect square.
 * https://oeis.org/A273368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even) ? 10*(n/2)^2 + 6*(n/2) : 10*((n+1)/2)^2 -
 *            6*((n+1)/2)
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use bit‑shifts to avoid overflow when n is the maximum value */
    idx_t half = n >> 1;            /* floor(n/2)  */
    if ((n & 1) == 0)               /* n is even   */
    {
        seq_t t = (seq_t)half;      /* m = n/2     */
        return 10 * t * t + 6 * t;
    }
    else                            /* n is odd    */
    {
        seq_t m = (seq_t)(half + 1);/* m = (n+1)/2 */
        return 10 * m * m - 6 * m;
    }
}
