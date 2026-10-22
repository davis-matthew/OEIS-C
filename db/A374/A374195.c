/*
 * A374195  Row sums of A373917.
 * https://oeis.org/A374195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Sum_{k=1..n-1} (k*10 mod n)
 * Range:     u8 n <= 23, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, though offset is 1 */
    seq_t sum = 0;
    if (n == 1) return sum;        /* a(1) = 0 */
    idx_t k;
    for (k = 1; k < n; ++k) {
        sum += ( (seq_t)k * 10 ) % (seq_t)n;
    }
    return sum;
}
