/*
 * A276377  60th powers: a(n) = n^60.
 * https://oeis.org/A276377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^60
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)n;
    seq_t result = 1;
    idx_t e = 60;
    while (e) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}
