/*
 * A306771  Numbers m such that m = i + j = i * k and phi(m) = phi(i) + phi(j)
 *          = phi(i) * phi(k) for some i, j, k, where phi is the Euler totient
 *          function A000010.
 * https://oeis.org/A306771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3*(2*n - 1) + (-1)^n)/2
 * Range:     u8 n <= 29, u16 n <= 7282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t two_n_minus1 = n_seq * (seq_t)2 - (seq_t)1;   /* 2*n - 1 */
    seq_t inner = two_n_minus1 * (seq_t)3;               /* 3*(2*n - 1) */
    if (n & (idx_t)1) {                                 /* n odd: subtract 1 */
        inner -= (seq_t)1;
    } else {                                            /* n even: add 1 */
        inner += (seq_t)1;
    }
    seq_t result = inner * (seq_t)3 / (seq_t)2;          /* 3*inner/2 */
    return result;
}
