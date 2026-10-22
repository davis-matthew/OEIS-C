/*
 * A327708  The minimal size of a partition lambda of n such that every
 *          partition of n with at most 8 parts can be obtained by coalescing
 *          the parts of lambda.
 * https://oeis.org/A327708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(floor(7*n/8)), with a(0)=0
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    while (n > 0) {
        ++res;
        n = (idx_t)((seq_t)n * 7 / 8);
    }
    return res;
}
