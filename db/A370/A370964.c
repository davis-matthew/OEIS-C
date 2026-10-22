/*
 * A370964  a(n) = number of "weakly lonesum" 2 X 2 X n tensors.
 * https://oeis.org/A370964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*15^n - 14^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t power15 = (seq_t)1;
    seq_t power14 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n; ++i) {
        power15 *= (seq_t)15;
        power14 *= (seq_t)14;
    }
    return power15 * ((seq_t)2) - power14;
}
