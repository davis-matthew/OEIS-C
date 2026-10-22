/*
 * A332410  a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) with a(0)=0,
 *          a(1)=1, a(2)=3, a(3)=6, a(4)=11, a(5)=17, a(6)=24.
 * https://oeis.org/A332410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) with
 *            a(0)=0, a(1)=1, a(2)=3, a(3)=6, a(4)=11, a(5)=17, a(6)=24
 * Range:     u8 n <= 20, u16 n <= 330, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle initial values directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)11;
    if (n == 5) return (seq_t)17;
    if (n == 6) return (seq_t)24;

    /* Circular buffer holding the last seven terms */
    seq_t a[7];
    a[0] = (seq_t)0;
    a[1] = (seq_t)1;
    a[2] = (seq_t)3;
    a[3] = (seq_t)6;
    a[4] = (seq_t)11;
    a[5] = (seq_t)17;
    a[6] = (seq_t)24;

    for (idx_t i = 7; i <= n; ++i) {
        seq_t val = ((seq_t)2) * a[6]      /* 2*a(n-1) */
                  - a[5]                  /* -a(n-2)   */
                  + a[0]                  /* +a(n-5)   */
                  - ((seq_t)2) * a[1]     /* -2*a(n-6) */
                  + a[2];                 /* +a(n-7)   */

        /* Shift buffer left: a[0] becomes a[1], ..., a[6] becomes val */
        a[0] = a[1];
        a[1] = a[2];
        a[2] = a[3];
        a[3] = a[4];
        a[4] = a[5];
        a[5] = a[6];
        a[6] = val;
    }
    return a[6];
}
