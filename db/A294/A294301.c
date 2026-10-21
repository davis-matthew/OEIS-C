/*
 * A294301  Sum of the sixth powers of the parts in the partitions of n into
 *          two distinct parts.
 * https://oeis.org/A294301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..⌊(n-1)/2⌋} (i^6 + (n-i)^6) (OEIS A294301)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 31, u64 n <= 746, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* sum over all unordered partitions of n into two distinct parts */
    idx_t max_i = (n - 1) >> 1;             /* floor((n-1)/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= max_i; ++i) {
        seq_t ix = (seq_t)i;                /* ith part as seq_t */
        seq_t i2 = ix * ix;                 /* i^2 */
        seq_t i4 = i2 * i2;                 /* i^4 */
        seq_t i6 = i4 * i2;                 /* i^6 */

        idx_t j = n - i;                    /* second part */
        seq_t jx = (seq_t)j;
        seq_t j2 = jx * jx;                 /* j^2 */
        seq_t j4 = j2 * j2;                 /* j^4 */
        seq_t j6 = j4 * j2;                 /* j^6 */

        sum += i6 + j6;
    }

    return sum;
}
