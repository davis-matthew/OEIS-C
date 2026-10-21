/*
 * A269110  Numbers of unit circles packed in a triangle of smallest area
 *          admitting an equilateral triangle solution.
 * https://oeis.org/A269110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(2k) = (k+1)(k+2)/2, a(2k-1) = k(k+3)/2 for k≥1 with the single
 *            special case a(1)=1
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, return 0 */
    if (n == 1) return (seq_t)1;          /* special case */

    if ((n & 1ULL) == 0) {               /* n even: n = 2k */
        seq_t k = (seq_t)(n / 2);        /* k = n/2 */
        seq_t m = k + (seq_t)1;         /* m = k+1 */
        return m * (m + (seq_t)1) / (seq_t)2;
    } else {                             /* n odd: n = 2k-1, k = (n+1)/2 */
        seq_t k = (seq_t)((n + 1) / 2);  /* k = (n+1)/2 */
        return k * (k + (seq_t)3) / (seq_t)2;
    }
}
