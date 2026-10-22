/*
 * A371704  Heights at which a Minecraft boat-drop will break up.
 * https://oeis.org/A371704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (50*n^2 + 50*n + 23 - 23*(-1)^n*(1+2*n)) / 16
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = 50 * nn * nn;
    val += 50 * nn;
    val += 23;
    seq_t term = 23 * (1 + 2 * nn);
    if (n & 1) {
        val += term;
    } else {
        val -= term;
    }
    return val / 16;
}
