/*
 * A287393  Domination number for knight graph on a 2 X n board.
 * https://oeis.org/A287393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(floor((n+4)/6) + floor((n+5)/6))
 * Range:     u8 n <= 379, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t rem = n % 6;
    idx_t t1  = n / 6 + (rem >= 2 ? 1 : 0);
    idx_t t2  = n / 6 + (rem != 0 ? 1 : 0);
    return (seq_t)2 * ((seq_t)t1 + (seq_t)t2);
}
