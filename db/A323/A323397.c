/*
 * A323397  a(n) = (4^n + 15*n - 1)/9.
 * https://oeis.org/A323397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n + 15*n - 1)/9 Recurrence: a(0) = 0 a(n+1) = 4*a(n) -
 *            5*n + 2 This implementation uses the recurrence, which keeps
 *            intermediate values within the range of the target type for all
 *            n whose final result fits in that type. For larger n the values
 *            may overflow, but the sequence values excee...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t val = (seq_t)0;          /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        /* compute a(i+1) = 4*a(i) - 5*i + 2 */
        val = val * (seq_t)4 - (seq_t)5 * (seq_t)i + (seq_t)2;
    }
    return val;
}
