/*
 * A248076  Partial sums of A001160: Sum_{j=1..n} sigma_5(j).
 * https://oeis.org/A248076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} sigma_5(k) (Using the identity sigma_5(k) =
 *            Sum_{d|k} d^5) Equivalent to a(n) = Sum_{k=1..n} k^5 *
 *            floor(n/k)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 53, u64 n <= 2184,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t kk = (seq_t)k;
        seq_t k2 = kk * kk;    /* k^2 */
        seq_t k3 = k2 * kk;    /* k^3 */
        seq_t k4 = k3 * kk;    /* k^4 */
        seq_t k5 = k4 * kk;    /* k^5 */
        seq_t m   = (seq_t)(n / k);  /* floor(n/k) */
        sum += k5 * m;
    }
    return sum;
}
