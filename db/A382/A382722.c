/*
 * A382722  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 13.
 * https://oeis.org/A382722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ (dᵢ + 1), where n = (…d₂d₁d₀)_13
 * Range:     u8 n <= 297, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prod = 1;
    while (n > 0) {
        prod = prod * ((seq_t)(n % 13) + 1);
        n = n / 13;
    }
    return prod;
}
