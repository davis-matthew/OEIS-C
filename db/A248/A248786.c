/*
 * A248786  a(n) = 29*n + floor(n/29) + 0^n - 0^(n mod 29).
 * https://oeis.org/A248786
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 29*n + floor(n/29) + 0^n - 0^(n mod 29)
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 29*n + n/29 - (n > 0 && n%29 == 0 ? 1 : 0) */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t domain */
    seq_t res = nn * 29;          /* 29*n */
    res += nn / 29;               /* + floor(n/29) */
    if (n != 0 && (n % 29) == 0)  /* adjust for multiples of 29, except n=0 */
        res -= 1;                 /* subtract 1 */
    return res;
}
