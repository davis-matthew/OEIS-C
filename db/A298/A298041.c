/*
 * A298041  Partial sums of A298040.
 * https://oeis.org/A298041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1 a(n) = a(n-1) + 4 if n == 1 a(n) = a(n-1) + 10 * n if n
 *            is even (n > 1) a(n) = a(n-1) + 8 * n if n is odd (n > 1)
 *            (Equivalently: for n = 2m: a(n) = 18m² + 10m – 3; for n = 2m+1:
 *            a(n) = 18m² + 26m + 5)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t m = n >> 1;             /* m = floor(n/2)          */
    seq_t m_t = (seq_t)m;

    if ((n & 1) == 0) {           /* even n = 2m             */
        /* a(2m) = 18*m² + 10*m – 3  = (18*m² + 10*m + 5) – 8  */
        seq_t t = 18 * m_t * m_t + 10 * m_t + 5;
        return t - 8;
    } else {                      /* odd n = 2m+1            */
        /* a(2m+1) = 18*m² + 26*m + 5  */
        return 18 * m_t * m_t + 26 * m_t + 5;
    }
}
