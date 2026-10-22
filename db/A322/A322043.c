/*
 * A322043  Numbers k such that the coefficient of x^k in the expansion of
 *          Product_{m >= 1} (1-x^m)^15 is zero.
 * https://oeis.org/A322043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (429*n^2 - 429*n + 106) / 2
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 216, u64 n <= 216,
 *            u128 n <= 216, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t val  = 429 * nn * nn - 429 * nn + 106;
    val /= 2;
    return val;
}
