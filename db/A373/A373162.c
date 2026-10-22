/*
 * A373162  a(n) = denominator of Sum_{k>=0} cos(2k*Pi/3)/n^k.
 * https://oeis.org/A373162
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2*(n^2+n+1) / gcd(6, n+2)
 * Range:     u8 n <= 10, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute d = gcd(6, n+2) using only the residue mod 6   */
    idx_t mod6 = ((n % 6) + 2) % 6;   /* (n+2) mod 6, avoiding overflow    */
    seq_t d;
    switch (mod6) {
        case 0: d = (seq_t)6; break;
        case 3: d = (seq_t)3; break;
        case 2:
        case 4: d = (seq_t)2; break;
        default: d = (seq_t)1; break;
    }

    /* Compute t = n^2 + n + 1 */
    seq_t N = (seq_t)n;
    seq_t t = N * (N + (seq_t)1) + (seq_t)1;

    /* a(n) = 2*t / d */
    return ((seq_t)2 * t) / d;
}
