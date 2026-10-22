/*
 * A337985  a(n) is the exponent of the highest power of 2 dividing the n-th
 *          Bell number.
 * https://oeis.org/A337985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 2 or 11 (mod 12); 2 if n ≡ 5 or 8 (mod 12); 0
 *            otherwise
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t r = n % (idx_t)12;
    switch (r) {
        case 2: case 11: return (seq_t)1;
        case 5: case 8: return (seq_t)2;
        default:         return (seq_t)0;
    }
}
