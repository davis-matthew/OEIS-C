/*
 * A329583  Numerators of 1 + n^2/4 + period 3: repeat [-1, 1, 1].
 * https://oeis.org/A329583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 4)*(5 - 3*(-1)^n)/8 + (-1)^((n+1) mod 3)
 *            Simplification used: if n is even: a(n) = (n^2 + 4)/4 + ((n+1)%3
 *            == 1 ? -1 : 1) if n is odd : a(n) = n^2 + 4 + ((n+1)%3 == 1 ? -1
 *            : 1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;
    seq_t n_sq = sn * sn;        /* n^2   */

    /* Base value depending on parity of n */
    seq_t base;
    if ((n & 1) == 0)            /* n even */
        base = (n_sq + 4) / 4;   /* (n^2+4)/4 */
    else
        base = n_sq + 4;         /* n^2+4   */

    /* Value of (-1)^((n+1) mod 3) :  -1 when ((n+1)%3)==1, else +1 */
    idx_t r = (n + 1) % 3;
    if (r == 1)
        return base - 1;         /* subtract 1 */
    else
        return base + 1;         /* add 1 */
}
