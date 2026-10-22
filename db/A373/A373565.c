/*
 * A373565  Expansion of x + 1/(1 - x).
 * https://oeis.org/A373565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^n] (x^2 - x - 1)/(x - 1) (equivalently: a(0)=1,
 *            a(1)=2, a(n)=1 for n>=2)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    return 1;
}
