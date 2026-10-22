/*
 * A368790  a(n) = (3*n-2)!!! * Sum_{k=0..n} 1/(3*k-2)!!!.
 * https://oeis.org/A368790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n-2)*a(n-1) + 1, with a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t factor = ((seq_t)3 * i) - 2;
        val = factor * val + 1;
    }
    return val;
}
