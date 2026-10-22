/*
 * A309686  Sum of the even parts appearing among the smallest parts of the
 *          partitions of n into 3 parts.
 * https://oeis.org/A309686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j even, 1 <= j <= floor(n/3)} j * ( floor((n-j)/2) -
 *            j + 1 ) This equals the sum of all even smallest parts j of the
 *            partitions of n into 3 parts
 * Range:     u8 n <= 37, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* j ranges over even integers from 2 up to floor(n/3). */
    for (idx_t j = 2; j <= n / 3; j += 2) {
        /* For given j, i goes from j to floor((n-j)/2). */
        idx_t max_i = (n - j) / 2;
        idx_t count_i = max_i - j + 1;          /* >= 0 because j <= floor(n/3) */
        res += (seq_t)j * (seq_t)count_i;
    }
    return res;
}
