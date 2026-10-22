/*
 * A387655  a(n) = numerator(n^3/3).
 * https://oeis.org/A387655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac, mult
 * Formula:   a(n) = numerator(n^3/3) = n^3 / (3 if 3 divides n, otherwise 1)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * nn * nn;
    if (n % 3 == 0)
        prod /= (seq_t)3;
    return prod;
}
