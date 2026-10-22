/*
 * A323610  List of 5-powerful numbers (for the definition of k-powerful see
 *          A323395).
 * https://oeis.org/A323610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 48 ; for n ≥ 2, a(n) = 8 × (n + 6)
 * Range:     u8 n <= 25, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of 48 followed by all multiples of 8 ≥ 64. */
seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)48;
    } else {
        return (seq_t)8 * ((seq_t)n + (seq_t)6);
    }
}
