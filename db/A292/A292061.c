/*
 * A292061  a(n) = (n-3)*(n-2)^2*(n-1)^2*n/24.
 * https://oeis.org/A292061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(n-2)^2*(n-1)^2*n/24
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    /* Promote n to seq_t once to use in the calculation */
    seq_t N            = (seq_t)n;
    seq_t n_minus_1    = N - (seq_t)1;
    seq_t n_minus_2    = N - (seq_t)2;

    seq_t result = N * (N - (seq_t)3);
    result *= n_minus_1;
    result *= n_minus_1;
    result *= n_minus_2;
    result *= n_minus_2;
    result /= (seq_t)24;

    return result;
}
