/*
 * A374984  a(n) = (1/4)*Product_{k=0..n} F(k)+4, where F=A000045 (Fibonacci
 *          numbers).
 * https://oeis.org/A374984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{k=0..n} (F(k)+4)) / 4 (F(k) is the kth Fibonacci
 *            number)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t product = 1;
    seq_t fprev = 0;
    seq_t fcurr = 1;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term   = fprev + ((seq_t)4);
        product     *= term;
        seq_t fnext = fprev + fcurr;
        fprev = fcurr;
        fcurr = fnext;
    }
    return product / ((seq_t)4);
}
