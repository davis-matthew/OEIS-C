/*
 * A303488  a(n) = n! * [x^n] 1/(1 - 5*x)^(n/5).
 * https://oeis.org/A303488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n-1} (5*k + n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t res = (seq_t)1;
    for (idx_t k = 0; k < n; ++k) {
        seq_t factor = (seq_t)n + ((seq_t)5 * (seq_t)k);
        res *= factor;
    }
    return res;
}
