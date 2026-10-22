/*
 * A332569  a(n) = Sum_{k=1..n} floor(n/k) * ceiling(n/k).
 * https://oeis.org/A332569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k) * ceil(n/k)
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t q = (seq_t)n / (seq_t)k;          /* floor(n/k)   */
        seq_t r = (seq_t)n % (seq_t)k;          /* remainder    */
        seq_t ceil = q + (r ? 1 : 0);           /* ceil(n/k)    */
        total += q * ceil;                      /* accumulate   */
    }
    return total;
}
