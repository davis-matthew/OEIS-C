/*
 * A332872  Number of ordered set partitions of {1..n} where no element of any
 *          block is greater than any element of a non-adjacent consecutive
 *          block.
 * https://oeis.org/A332872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=1, a(2)=3; for n>=3: a(n)=4*a(n-1)-2*a(n-2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 3;

    seq_t prev2 = 1;   /* a(1) */
    seq_t prev1 = 3;   /* a(2) */
    seq_t cur;

    for (idx_t k = 3; k <= n; ++k) {
        cur = prev1 * 4 - prev2 * 2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
