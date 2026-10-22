/*
 * A333772  a(n) = n * 2^n * (n!)^2.
 * https://oeis.org/A333772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * 2^n * (n!)^2 (OEIS A333772)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is not defined in the OEIS offset, return 0 for safety */
    if (n == 0) {
        return 0;
    }

    /* Start from a(1) = 2 */
    seq_t res = 2;

    /* Recurrence: a(k) = a(k-1) * 2 * k^3 / (k-1)  for k >= 2 */
    for (idx_t k = 2; k <= n; ++k) {
        res *= (seq_t)2;           /* multiply by 2         */
        res *= (seq_t)k;           /* multiply by k         */
        res *= (seq_t)k;           /* multiply by k again   */
        res *= (seq_t)k;           /* multiply by k third   */
        res /= (seq_t)(k - 1);     /* divide by (k-1) (exact) */
    }

    return res;
}
