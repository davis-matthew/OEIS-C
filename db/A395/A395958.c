/*
 * A395958  Third iterate of the Thue-Morse transform applied to A000035.
 * https://oeis.org/A395958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(16*n + r) = a(n) XOR a(r) for 0 <= r < 16, with base a(0..15)
 *            = {0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0}
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base[16] = {
    0, 1, 1, 0,
    0, 1, 1, 0,
    0, 1, 1, 0,
    0, 1, 1, 0
};

seq_t a(idx_t n)
{
    if (n < 16) return base[n];
    return a(n / 16) ^ a(n % 16);
}
