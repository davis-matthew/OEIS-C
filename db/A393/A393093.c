/*
 * A393093  Group the partitions of n into 3 parts into sets whose smallest
 *          parts are equal. a(n) is the sum of the maximum values of all the
 *          middle parts in each set.
 * https://oeis.org/A393093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=1..floor(n/3)} floor((n-j)/2)
 * Range:     u8 n <= 44, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t maxj = n / 3;          /* floor(n/3) */
    seq_t sum = 0;
    for (idx_t j = 1; j <= maxj; ++j) {
        sum += (seq_t)(n - j) / 2;
    }
    return sum;
}
