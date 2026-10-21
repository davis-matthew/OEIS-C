/*
 * A276395  Characteristic function of floor(36*n/25).
 * https://oeis.org/A276395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the periodic binary pattern of floor(36*n/25), with
 *            period 36: period = 110110110111011011011011101101101101
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[36] = {
        1,1,0,1,1,0,1,1,0,1,1,1,0,1,1,0,1,1,0,1,1,0,1,1,
        1,0,1,1,0,1,1,0,1,1,0,1
    };
    idx_t i = (n - 1) % 36;
    return period[i];
}
