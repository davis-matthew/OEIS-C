/*
 * A255435  Product_{k=0..n} (k^5 + 1).
 * https://oeis.org/A255435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0..n} (k^5 + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t base   = (seq_t)k;
        seq_t term   = base * base * base * base * base + 1;
        result       = result * term;
    }
    return result;
}
