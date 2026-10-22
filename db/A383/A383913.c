/*
 * A383913  Expansion of (1+x) * (1+2*x) * (1+3*x)/((1-x) * (1-2*x) * (1-3*x)
 *          * (1-4*x)).
 * https://oeis.org/A383913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 35*a(n-2) + 50*a(n-3) - 24*a(n-4) Initial
 *            values: a(0)=1, a(1)=16, a(2)=136, a(3)=856
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first four known values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)136;
    if (n == 3) return (seq_t)856;

    /* Variables to hold a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(n-4) */
    seq_t a1 = (seq_t)16;  /* a(n-3) */
    seq_t a2 = (seq_t)136; /* a(n-2) */
    seq_t a3 = (seq_t)856; /* a(n-1) */

    /* Compute forward up to the requested n */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)10 * a3
                    - (seq_t)35 * a2
                    + (seq_t)50 * a1
                    - (seq_t)24 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
