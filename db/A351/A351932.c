/*
 * A351932  Number of set partitions of [n] such that block sizes are either 1
 *          or 4.
 * https://oeis.org/A351932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + binom(n-1,3) * a(n-4) for n > 3 with initial
 *            values a(0) = a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) {
        return (seq_t)1;
    }
    seq_t a0 = 1, a1 = 1, a2 = 1, a3 = 1, cur;
    for (idx_t i = 4; i <= n; ++i) {
        seq_t comb = ((seq_t)(i - 1) * (seq_t)(i - 2) * (seq_t)(i - 3)) / (seq_t)6;
        cur = a3 + comb * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = cur;
    }
    return a3;
}
