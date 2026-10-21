/*
 * A261882  Decimal expansion of 32/27.
 * https://oeis.org/A261882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = 1 if n <= 2; else a(n) = 8,5,1 repeated (a(n) =
 *            {8,5,1}[(n-3) mod 3])
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) {
        return (seq_t)1;
    }
    idx_t r = (n - 3) % 3;
    switch (r) {
        case 0: return (seq_t)8;
        case 1: return (seq_t)5;
        default: return (seq_t)1;
    }
}
