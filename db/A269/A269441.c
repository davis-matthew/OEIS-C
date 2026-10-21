/*
 * A269441  Alternating sum of 10-gonal (or decagonal) pyramidal numbers.
 * https://oeis.org/A269441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((-1)^n*(16*n^3 + 30*n^2 - 4*n - 9) + 9) / 24
 * Range:     i8 n <= 1, i16 n <= 12, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nsi = (seq_t)n;
    seq_t n2  = nsi * nsi;          /* n^2 */
    seq_t n3  = n2 * nsi;           /* n^3 */
    seq_t term1 = 16 * n3;          /* 16*n^3 */
    seq_t term2 = 30 * n2;          /* 30*n^2 */
    seq_t term3 = -4 * nsi;         /* -4*n   */
    seq_t term4 = -9;               /* -9     */
    seq_t sum   = term1 + term2 + term3 + term4;
    seq_t sign  = (n & 1ULL) ? -1 : 1;   /* (-1)^n  */
    seq_t num   = sign * sum + 9;
    return num / 24;
}
