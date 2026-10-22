/*
 * A308266  Sum of the middle parts in the partitions of n into 3 parts.
 * https://oeis.org/A308266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..⌊n/3⌋} Sum_{i=k..⌊(n-k)/2⌋} i
 * Range:     u8 n <= 22, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= n / 3; ++k) {
        idx_t m = (n - k) / 2;            /* upper bound for the inner sum */
        seq_t sum_m  = ((seq_t)m * ((seq_t)m + 1)) / 2;
        seq_t sum_k1 = ((seq_t)(k - 1) * (seq_t)k) / 2;
        res += sum_m - sum_k1;
    }
    return res;
}
