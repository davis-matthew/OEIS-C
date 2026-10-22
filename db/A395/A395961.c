/*
 * A395961  Fourth iterate of the Thue-Morse transform applied to A000035.
 * https://oeis.org/A395961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = parity of bits of n whose positions are multiples of 4
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 0; k < 64; k += 4) {
        res ^= (seq_t)((n >> k) & 1);
    }
    return res;
}
