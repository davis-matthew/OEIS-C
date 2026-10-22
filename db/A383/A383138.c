/*
 * A383138  a(n) = Sum_{k=0..n} k^4 * 2^(n-k) * binomial(n,k).
 * https://oeis.org/A383138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-4) * n * (-6 + 20*n + 12*n^2 + n^3) for n >= 4;
 *            a(0)=0, a(1)=1, a(2)=20, a(3)=189
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 31, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 20;
    if (n == 3) return 189;

    /* Compute 3^(n-4) */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n - 4; ++i)
        pow3 *= 3;

    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;
    seq_t n3 = n2 * n_seq;
    seq_t coef = n3 + 12 * n2 + 20 * n_seq - 6;  /* always positive for n >= 4 */

    return pow3 * n_seq * coef;
}
