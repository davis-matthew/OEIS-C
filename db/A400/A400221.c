/*
 * A400221  Number of divisors of 3360^n.
 * https://oeis.org/A400221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n+1)^3*(5*n+1)
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 170, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;
    seq_t n1   = sn + 1;
    seq_t cube = n1 * n1 * n1;
    seq_t fac  = (5 * sn) + 1;
    return cube * fac;
}
