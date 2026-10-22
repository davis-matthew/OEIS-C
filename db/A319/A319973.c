/*
 * A319973  Expansion of (1-x^5+x^8+x^10)/((1-x^4)*(1-x^5)*(1-x^6)).
 * https://oeis.org/A319973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2+12*n+72)/120 - (n mod 2)*n/6 + ((n^2+2*n+2)
 *            mod 5)/5)
 * Range:     u8 n <= 7, u16 n <= 388, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t A = nn * nn + 12 * nn + 72;                 /* (n^2 + 12n + 72) */
    seq_t B = ((seq_t)(n & 1)) * nn * 20;             /* (n mod 2) * n * 20  (since /6 is *20/120) */
    seq_t C = ((nn * nn + 2 * nn + 2) % 5) * 24;      /* ((n^2 + 2n + 2) mod 5) * 24 (since /5 is *24/120) */
    seq_t N = A - B + C;
    return N / 120;                                   /* floor division by 120 */
}
