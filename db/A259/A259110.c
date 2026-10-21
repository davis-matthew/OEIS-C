/*
 * A259110  a(n) = 2*A000447(n).
 * https://oeis.org/A259110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(4*n^2-1)/3
 * Range:     u8 n <= 4, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t tn = (seq_t)n;            /* n as seq_t */
    seq_t two_n = tn * 2;           /* 2*n */
    seq_t two_n_minus1 = two_n - 1; /* 2*n - 1 */
    seq_t two_n_plus1 = two_n + 1;  /* 2*n + 1 */

    /* Divide one of the three consecutive factors by 3 */
    if (two_n % 3 == 0) {
        two_n /= 3;
    } else if (two_n_minus1 % 3 == 0) {
        two_n_minus1 /= 3;
    } else {
        two_n_plus1 /= 3;
    }

    seq_t res = two_n * two_n_minus1;
    res *= two_n_plus1;
    return res;
}
