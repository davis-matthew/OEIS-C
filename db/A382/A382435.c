/*
 * A382435  a(n) = Sum_{k=0..n} ( binomial(n,k) - binomial(n,k-1) )^6.
 * https://oeis.org/A382435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} ( binomial(n,k) - binomial(n,k-1) )^6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t b_prev = 1;
    seq_t diff_abs = b_prev;
    seq_t pow6 = diff_abs * diff_abs * diff_abs * diff_abs * diff_abs * diff_abs;
    sum += pow6;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t mult = (seq_t)(n - k + 1);
        seq_t div = (seq_t)k;
        seq_t b_curr = b_prev * mult / div;
        diff_abs = (b_curr > b_prev) ? (b_curr - b_prev) : (b_prev - b_curr);
        pow6 = diff_abs * diff_abs * diff_abs * diff_abs * diff_abs * diff_abs;
        sum += pow6;
        b_prev = b_curr;
    }
    return sum;
}
