/*
 * A254640  Third partial sums of sixth powers (A001014).
 * https://oeis.org/A254640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n+3)*(3+2*n)*(2 -30*n +35*n^2 +30*n^3
 *            +5*n^4)/5040
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 22, u64 n <= 274, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A254640 – third partial sums of sixth powers. */
seq_t a(idx_t n)
{
    /* Use 128‑bit signed arithmetic for safe intermediate products. */
    __int128 nn   = (long long)n;                     /* cast n to signed 128‑bit */
    __int128 fact = nn * (nn + 1) * (nn + 2) * (nn + 3) *
                    (3 + 2 * nn);                      /* n*(n+1)*(n+2)*(n+3)*(3+2n) */
    __int128 poly = 2
                    - 30 * nn
                    + 35 * nn * nn
                    + 30 * nn * nn * nn
                    + 5  * nn * nn * nn * nn;         /* 2 – 30n + 35n² + 30n³ + 5n⁴ */
    __int128 num  = fact * poly;                     /* numerator */
    return (seq_t)(num / 5040);                      /* divide by 5040, cast to seq_t */
}
