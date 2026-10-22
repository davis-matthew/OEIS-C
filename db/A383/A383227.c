/*
 * A383227  a(n) is the product of first n even numbers not divisible by 5
 *          (cf. A217562).
 * https://oeis.org/A383227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} 2*(i + ⌊(i-1)/4⌋)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        /* term = 2 * (i + floor((i-1)/4)) */
        seq_t term = 2 * ((seq_t)i + (seq_t)((i - 1) / 4));
        prod *= term;
    }
    return prod;
}
