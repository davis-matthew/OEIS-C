/*
 * A356397  a(n) is the product of the terms in the n-th row of triangle
 *          A343835; a(0) = 1.
 * https://oeis.org/A356397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∏_{runs of consecutive 1's in binary(n)} (2^l - 1) * 2^s
 *            where s is the starting bit position and l is the run length
 * Range:     u8 n <= 39, u16 n <= 169, u32 n <= 2643, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t temp = n;
    int pos = 0;                    /* current bit position in the original n   */
    while (temp != 0) {
        if (temp & 1) {
            /* start of a new run of 1's */
            int len = 0;
            while (temp & 1) {
                temp >>= 1;
                ++len;
            }
            /* value contributed by this run: (2^len - 1) shifted left by pos */
            seq_t block = ((seq_t)1 << len) - 1;
            block <<= pos;
            prod *= block;
            pos += len;
        } else {
            temp >>= 1;
            ++pos;
        }
    }
    return prod;
}
