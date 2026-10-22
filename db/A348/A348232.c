/*
 * A348232  Partial sums of A348231.
 * https://oeis.org/A348232
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 8*floor(n/2)*(floor(n/2)+1) + 10*ceil(n/2)^2 -
 *            2*ceil(n/2)
 * Range:     u8 n <= 7, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) and ceil(n/2) as seq_t */
    seq_t n_even = (seq_t)(n / 2);          /* floor(n/2) */
    seq_t n_odd  = (seq_t)(n - n_even);     /* ceil(n/2) = n - floor(n/2) */

    /* sum of even indices up to n: 2+4+...+2*n_even = n_even*(n_even+1) */
    seq_t sum_even = n_even * (n_even + 1);

    /* sum of odd indices up to n: 1+3+... = n_odd^2 (sum of first n_odd odd numbers) */
    seq_t sum_odd = n_odd * n_odd;

    /* combine according to the derived closed form */
    return (seq_t)1
           + (seq_t)8 * sum_even
           + (seq_t)10 * sum_odd
           - (seq_t)2 * n_odd;
}
