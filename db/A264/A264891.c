/*
 * A264891  a(n) = n*(5*n - 3)*(25*n^2 - 15*n - 6)/8.
 * https://oeis.org/A264891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n - 3)*(25*n^2 - 15*n - 6)/8
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 84, u64 n <= 1200,
 *            u128 n <= 1200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = n;
    seq_t t1 = n_t;
    seq_t t2 = (seq_t)5 * n_t - (seq_t)3;
    seq_t t3 = (seq_t)25 * n_t * n_t - (seq_t)15 * n_t - (seq_t)6;
    seq_t prod = t1 * t2;
    prod = prod * t3;
    return prod / (seq_t)8;
}
