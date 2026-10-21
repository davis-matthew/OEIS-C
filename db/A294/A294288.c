/*
 * A294288  Sum of the fourth powers of the parts in the partitions of n into
 *          two distinct parts.
 * https://oeis.org/A294288
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} (i^4 + (n-i)^4)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 117, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3u)          /* for n = 1,2 the sum is empty */
        return 0;

    idx_t limit = (n - 1) >> 1;   /* floor((n-1)/2) */
    seq_t sum = 0;

    for (idx_t i = 1; i <= limit; ++i) {
        seq_t ii = (seq_t)i;
        seq_t i2 = ii * ii;
        seq_t i4 = i2 * i2;

        idx_t rem_i = n - i;
        seq_t rr = (seq_t)rem_i;
        seq_t r2 = rr * rr;
        seq_t r4 = r2 * r2;

        sum += i4 + r4;
    }

    return sum;
}
