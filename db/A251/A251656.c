/*
 * A251656  4-step Fibonacci sequence starting with 1,0,1,0.
 * https://oeis.org/A251656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = a(n) + a(n+1) + a(n+2) + a(n+3), initial values:
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=0
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)0;

    seq_t s0 = (seq_t)1;
    seq_t s1 = (seq_t)0;
    seq_t s2 = (seq_t)1;
    seq_t s3 = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = s0 + s1 + s2 + s3;
        s0 = s1;
        s1 = s2;
        s2 = s3;
        s3 = next;
    }
    return s3;
}
