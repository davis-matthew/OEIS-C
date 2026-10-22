/*
 * A328985  First differences of A328984.
 * https://oeis.org/A328985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(4k) = 5*k + 1 a(4k+1) = -(5*k - 2) a(4k+2) = 5*k + 4 a(4k+3) =
 *            -(5*k + 2) where k = floor(n/4) and offset starts at n = 1
 * Range:     i8 n <= 58, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* convert index to seq_t for computations */
    seq_t t;
    switch (n & 3U) {             /* n mod 4 */
    case 0:                       /* n ≡ 0 (mod 4) */
        t = tn / 4;
        return 5 * t + 1;
    case 1:                       /* n ≡ 1 (mod 4) */
        t = (tn - 1) / 4;
        return -(5 * t - 2);
    case 2:                       /* n ≡ 2 (mod 4) */
        t = (tn - 2) / 4;
        return 5 * t + 4;
    case 3:                       /* n ≡ 3 (mod 4) */
        t = (tn - 3) / 4;
        return -(5 * t + 2);
    }
    return 0;                     /* unreachable but keeps compiler happy */
}
