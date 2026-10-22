/*
 * A323541  a(n) = Product_{k=0..n} (k^3 + (n-k)^3).
 * https://oeis.org/A323541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^3 + (n-k)^3) a(0) = 0 This
 *            implementation multiplies each term exactly once. All arithmetic
 *            is performed in seq_t; indices are first promoted to seq_t to
 *            avoid intermediate overflow on the index values
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 is handled naturally by the loop, but an early return
       saves a mul by 0. */
    if (n == 0u) {
        return 0;
    }

    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t k_cu = (seq_t)k * k * k;
        idx_t nk   = n - k;
        seq_t nk_cu = (seq_t)nk * nk * nk;
        seq_t term   = k_cu + nk_cu;
        result *= term;
    }
    return result;
}
