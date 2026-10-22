/*
 * A368838  a(n) = n! * (n+3)! * Sum_{k=0..n} 1/(k! * (k+3)!).
 * https://oeis.org/A368838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+3)*a(n-1) + 1; a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff = (seq_t)i * ((seq_t)i + (seq_t)3);
        res = coeff * res + 1;
    }
    return res;
}
