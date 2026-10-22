/*
 * A391165  Primitive cubic pyritohedral numbers: a(n) = 864*n^3 - 2484*n^2 +
 *          2384*n - 763.
 * https://oeis.org/A391165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 864*n^3 - 2484*n^2 + 2384*n - 763 Offset: 1 (a(1)
 *            corresponds to n = 1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 171, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t result = 864 * nn * nn * nn;
    result -= 2484 * nn * nn;
    result += 2384 * nn;
    result -= 763;
    return result;
}
