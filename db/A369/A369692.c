/*
 * A369692  Connected domination number of the n X n grid graph.
 * https://oeis.org/A369692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 if n == 1 3 if n == 3 (n/3)*(n+1) if n % 3 == 0 and n >
 *            3 ((n+2)/3 - 1)*(3*(n+2)/3 + 1) if n % 3 == 1 and n > 1
 *            3*((n+1)/3)^2 - 1 if n % 3 == 2
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    idx_t r = n % 3;
    if (r == 0) {
        /* n = 3*k , k >= 2 */
        idx_t k = n / 3;
        return (seq_t)k * ((seq_t)n + 1);
    } else if (r == 1) {
        /* n = 3*m - 2 , m >= 2 */
        idx_t m = (n + 2) / 3;
        return ((seq_t)(m - 1)) * (((seq_t)3 * m) + 1);
    } else { /* r == 2 */
        /* n = 3*k - 1 , k >= 1 */
        idx_t k = (n + 1) / 3;
        seq_t kk = (seq_t)k * k;   /* k^2 */
        return (seq_t)3 * kk - 1;
    }
}
