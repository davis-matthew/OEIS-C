/*
 * A354801  n^2 minus the sum of all aliquot divisors of all positive integers
 *          <= n.
 * https://oeis.org/A354801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 - ∑_{i=1}^{n} i·⌊n/i⌋ + n(n+1)/2 (equivalent to n^2
 *            minus the sum of all aliquot divisors of integers ≤ n)
 * Range:     u8 n <= 19, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safety for out-of-range calls */

    seq_t sum_sigma = 0;
    for (idx_t i = 1; i <= n; ++i)
        sum_sigma += (seq_t)i * (n / i);          /* i appears floor(n/i) times */

    seq_t n_seq   = (seq_t)n;
    seq_t n_sq    = n_seq * n_seq;               /* n^2   */
    seq_t mid     = n_seq * (n_seq + 1) / 2;     /* n(n+1)/2 */

    return n_sq - sum_sigma + mid;
}
