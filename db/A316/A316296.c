/*
 * A316296  a(n) = Sum_{k=1..n} f(k, n), where f(i, j) is the number of
 *          multiples of i greater than j and less than 2*j.
 * https://oeis.org/A316296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} ( floor((2n-1)/k) - floor(n/k) )
 * Range:     u8 n <= 57, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t n_seq = (seq_t)n;
    /* 2*n - 1, computed in seq_t to avoid overflow */
    seq_t two_n_minus1 = n_seq * 2 - 1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k_seq = (seq_t)k;
        sum += (two_n_minus1 / k_seq) - (n_seq / k_seq);
    }
    return sum;
}
