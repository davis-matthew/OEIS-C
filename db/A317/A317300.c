/*
 * A317300  Sequence obtained by taking the general formula for generalized
 *          k-gonal numbers: m*((k - 2)*m - k + 4)/2, where m = 0, +1, -1, +2,
 *          -2, +3, -3, ... and k >= 5. Here k = 0.
 * https://oeis.org/A317300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = m*(2-m) with m = (n+1)/2 for odd n, m = -(n/2) for even
 *            n>0, a(0)=0 (direct formula derived from the generalized k-gonal
 *            numbers definition)
 * Range:     i8 n <= 21, i16 n <= 361, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t m;
    if (n & 1)            /* odd n */
        m = (seq_t)((n + 1) / 2);
    else                  /* even n (n>0) */
        m = -(seq_t)(n / 2);

    return m * ((seq_t)2 - m);
}
