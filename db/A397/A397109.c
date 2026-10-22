/*
 * A397109  Lower (1,1/2) midsequence of (floor[n/2]^2) and (ceiling[n/3]^2);
 *          see Comments.
 * https://oeis.org/A397109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((11*n^2+8*n+11)/36 - (n mod 2)*n/2 - ((n+2) mod
 *            3)*n/9)
 * Range:     u8 n <= 4, u16 n <= 324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = n;

    /* Compute the numerator of the rational expression. */
    seq_t num = (seq_t)11 * N * N + (seq_t)8 * N + (seq_t)11;

    /* (n mod 2)*n/2  → 18*(n mod 2)*n / 36 */
    idx_t mod2 = n & 1;
    seq_t t2 = (seq_t)18 * mod2 * N;       /* 18*(n mod 2)*n */

    /* ((n+2) mod 3)*n/9  → 4*((n+2) mod 3)*n / 36 */
    idx_t mod3 = (n + 2) % 3;
    seq_t t3 = (seq_t)4 * mod3 * N;        /* 4*((n+2) mod 3)*n */

    /* Final integer division by 36 gives the correct a(n). */
    return (num - t2 - t3) / (seq_t)36;
}
