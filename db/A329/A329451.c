/*
 * A329451  Maximum number of pieces that can be captured during one move on
 *          an n X n board according to the international draughts capture
 *          rules.
 * https://oeis.org/A329451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n is odd : ((n-1)/2)^2 if n ≡ 0 (mod 4) : n==0 -> 0
 *            n==4 -> 1 else 4*(n/4)^2 - 2*(n/4) - 2 if n ≡ 2 (mod 4) : n==2
 *            -> 0 else 4*((n-2)/4)^2 + 2*((n-2)/4) - 1
 * Range:     u8 n <= 32, u16 n <= 512, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* odd n: a(2t+1) = t^2 where t = (n-1)/2 */
    if ((n & 1) != 0) {
        idx_t t = (n - 1) >> 1;          /* (n-1)/2 */
        return (seq_t)t * t;
    }

    /* even n: distinguish modulo 4 */
    if ((n & 3) == 0) {                 /* n % 4 == 0 */
        if (n == 4) return 1;           /* exception n=4 */
        idx_t t = n >> 2;                /* n/4 */
        /* a = 4*t^2 - 2*t - 2 */
        return (seq_t)4 * t * t - 2 * t - 2;
    } else {                            /* n % 4 == 2 */
        if (n == 2) return 0;           /* exception n=2 */
        idx_t t = (n - 2) >> 2;          /* (n-2)/4 */
        /* a = 4*t^2 + 2*t - 1 */
        return (seq_t)4 * t * t + 2 * t - 1;
    }
}
