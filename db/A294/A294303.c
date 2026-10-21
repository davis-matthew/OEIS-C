/*
 * A294303  Sum of the eighth powers of the parts in the partitions of n into
 *          two distinct parts.
 * https://oeis.org/A294303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} (i^8 + (n-i)^8)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 176,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = (n - 1) / 2;          /* floor((n-1)/2) */
    seq_t sn = (seq_t)n;

    for (idx_t i = 1; i <= limit; ++i) {
        seq_t x = (seq_t)i;
        seq_t y = sn - x;               /* n - i */

        /* compute x^8 */
        seq_t p = x * x;                /* x^2   */
        p *= p;                         /* x^4   */
        p *= p;                         /* x^8   */

        /* compute y^8 */
        seq_t q = y * y;                /* y^2   */
        q *= q;                         /* y^4   */
        q *= q;                         /* y^8   */

        sum += p + q;
    }

    return sum;
}
