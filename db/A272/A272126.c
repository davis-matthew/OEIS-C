/*
 * A272126  a(n) = 120*n^3 + 60*n^2 + 2*n + 1.
 * https://oeis.org/A272126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120*n^3 + 60*n^2 + 2*n + 1
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 329, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* also a(n) = (2*n+1) * (60*n*n + 1) */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    seq_t t1 = tn * tn * 60 + 1;          /* 60*n^2 + 1 */
    seq_t t2 = tn * 2 + 1;               /* 2*n  + 1 */
    return t1 * t2;
}
