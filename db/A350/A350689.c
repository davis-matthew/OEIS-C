/*
 * A350689  a(n) = n*(1 - (-1)^n - 2*(3 + (-1)^n)*n^2 + 2*n^4)/384.
 * https://oeis.org/A350689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1 - (-1)^n - 2*(3 + (-1)^n)*n^2 + 2*n^4)/384
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use signed 128‑bit integers for intermediate calculations to avoid overflow */
    __int128 n128 = n;
    __int128 eps   = (n & 1) ? -1 : 1;          /* (-1)^n */
    __int128 n2    = n128 * n128;              /* n^2 */
    __int128 n4    = n2 * n2;                  /* n^4 */

    __int128 inner = 1 - eps
                     - 2 * ( ( (__int128)3 + eps) * n2 )
                     + 2 * n4;

    __int128 num   = n128 * inner;             /* numerator */
    __int128 res   = num / 384;                /* division, exact */

    return (seq_t)res;
}
