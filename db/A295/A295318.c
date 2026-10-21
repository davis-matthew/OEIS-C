/*
 * A295318  Sum of the products of the smaller and larger parts of the
 *          partitions of n into two distinct parts with the smaller part
 *          even.
 * https://oeis.org/A295318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i even, 2 ≤ i ≤ floor((n-1)/2)} i * (n - i) * Closed
 *            form: let K = floor((n-1)/4). Then * a(n) = n * K * (K + 1) - (
 *            2 * K * (K + 1) * (2*K + 1) ) / 3
 * Range:     u8 n <= 16, u16 n <= 110, u32 n <= 4296, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the upper bound for even i: i_max = floor((n-1)/2) */
    /* K is the number of even values i = 2,4,... up to i_max */
    idx_t K_idx = (n - 1) / 4;      /* floor((n-1)/4) */
    seq_t K = (seq_t)K_idx;
    seq_t n_t = (seq_t)n;

    /* term1 = n * K * (K + 1) */
    seq_t term1 = n_t * K * (K + 1);

    /* term2 = (2 * K * (K + 1) * (2*K + 1)) / 3 */
    seq_t term2 = K * (K + 1);           /* K*(K+1) */
    term2 = term2 * ((K << 1) + 1);      /* * (2*K + 1) */
    term2 = term2 * 2;                   /* *2 */
    term2 /= 3;                          /* divide by 3 (exact) */

    return term1 - term2;
}
