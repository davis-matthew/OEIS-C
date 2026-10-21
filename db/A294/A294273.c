/*
 * A294273  Sum of the sixth powers of the parts in the partitions of n into
 *          two parts.
 * https://oeis.org/A294273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (i^6 + (n-i)^6)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 31, u64 n <= 746, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t sum = 0;
    idx_t half = n >> 1;           /* floor(n/2) */
    for (idx_t i = 1; i <= half; ++i) {
        seq_t ii = (seq_t)i;
        seq_t i6 = ii * ii * ii * ii * ii * ii; /* i^6 */
        idx_t j = n - i;
        seq_t jj = (seq_t)j;
        seq_t j6 = jj * jj * jj * jj * jj * jj; /* (n-i)^6 */
        sum += i6 + j6;
    }
    return sum;
}
