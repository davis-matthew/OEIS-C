/*
 * A340670  Number of complex base i-1 points which can be represented within
 *          n bits and negated within those n bits.
 * https://oeis.org/A340670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + 8*a(n-5) Initial values: a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=3, a(4)=5
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)5;

    /* Buffer of size 6 to store a(n-5) … a(n-1) using a circular index */
    seq_t buf[6];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)1;   /* a(1) */
    buf[2] = (seq_t)1;   /* a(2) */
    buf[3] = (seq_t)3;   /* a(3) */
    buf[4] = (seq_t)5;   /* a(4) */

    /* Compute subsequent values up to n */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t val = buf[(i - 1) % 6];                   /* a(n-1) */
        val += (seq_t)2 * buf[(i - 3) % 6];             /* 2*a(n-3) */
        val += (seq_t)8 * buf[(i - 5) % 6];             /* 8*a(n-5) */
        buf[i % 6] = val;                               /* store a(n) */
    }

    return buf[n % 6];
}
