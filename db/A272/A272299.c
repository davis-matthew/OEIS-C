/*
 * A272299  a(n) = n + 2*floor(n/2) + 3*floor(n/3).
 * https://oeis.org/A272299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 2*floor(n/2) + 3*floor(n/3) Equivalent to a(n) = 3*n
 *            - (n mod 2) - (n mod 3)
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t m2 = (seq_t)(n % 2);
    seq_t m3 = (seq_t)(n % 3);
    return 3 * n_seq - m2 - m3;
}
