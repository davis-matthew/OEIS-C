/*
 * A300402  Smallest integer i such that TREE(i) >= n.
 * https://oeis.org/A300402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A300402: a(n) = 1 for n<=1,
 *                     a(n) = 2 for 2<=n<=3,
 *                     a(n) = 3 otherwise
 * (since TREE(3) is astronomically large). */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    if (n <= 3)
        return (seq_t)2;
    return (seq_t)3;
}
