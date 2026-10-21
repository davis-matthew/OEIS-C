/*
 * A294272  Sum of the fifth powers of the parts in the partitions of n into
 *          two parts.
 * https://oeis.org/A294272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (i^5 + (n-i)^5)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 54, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t i = 1; i <= half; ++i) {
        seq_t ii = (seq_t)i;
        seq_t p1 = ii * ii * ii * ii * ii;          /* i^5 */

        idx_t m = n - i;
        seq_t mm = (seq_t)m;
        seq_t p2 = mm * mm * mm * mm * mm;          /* (n-i)^5 */

        sum += p1 + p2;
    }
    return sum;
}
