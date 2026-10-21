/*
 * A266956  Numbers m such that 9*m+7 is a square.
 * https://oeis.org/A266956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*(n-1)*n - 7*(2*n-1)*(-1)^n + 1)/8 Implemented using a
 *            sign–free, piecewise formula: a(n) = 9*k*k + 8*k + 1 if n is odd
 *            (k = (n-1)/2) a(n) = 9*k*k +10*k + 2 if n is even (k = (n-2)/2)
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For safety, reject offset 0 – it is outside the domain of the sequence. */
    if (n == 0) return 0;

    if (n & 1U)    /* n is odd */
    {
        idx_t k = (n - 1) >> 1;               /* k = (n-1)/2  */
        seq_t kS = (seq_t)k;                  /* cast to seq_t  */
        seq_t kSq = kS * kS;                  /* k^2  */
        return 9 * kSq + 8 * kS + 1;          /* 9*k^2 + 8*k + 1 */
    }
    else            /* n is even */
    {
        idx_t k = (n - 2) >> 1;               /* k = (n-2)/2  */
        seq_t kS = (seq_t)k;
        seq_t kSq = kS * kS;
        return 9 * kSq + 10 * kS + 2;         /* 9*k^2 +10*k + 2 */
    }
}
