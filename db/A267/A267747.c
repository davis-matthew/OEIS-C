/*
 * A267747  Numbers k such that k mod 2 = k mod 3 = k mod 5.
 * https://oeis.org/A267747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n - 7*(-1)^n - 22
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = sn * (seq_t)15;
    if (n & 1)
        res += (seq_t)7;
    else
        res -= (seq_t)7;
    res -= (seq_t)22;
    return res;
}
