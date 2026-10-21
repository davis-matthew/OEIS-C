/*
 * A277641  Numbers k > 2 such that the Diophantine equation x^2 + 2^a * 5^b *
 *          13^c = y^k has one or more solutions for nonnegative a, b, c with
 *          x, y > 0 and gcd(x, y) = 1.
 * https://oeis.org/A277641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n + 2 + ((n + 2) >= 9 ? 3 : 0)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cand = (seq_t)n + 2;
    if (cand >= 9) cand += 3;
    return cand;
}
