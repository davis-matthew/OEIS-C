/*
 * A373358  a(n) = 4*a(n-1) -5*a(n-2) +2*a(n-3) +2*a(n-4) for a(0) = a(1) = 0,
 *          a(2) = 1, a(3) = 4 for n >= 4.
 * https://oeis.org/A373358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) -5*a(n-2) +2*a(n-3) +2*a(n-4); Initial values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=4;
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;

    /* successive values are needed only for the last four terms */
    seq_t p0 = (seq_t)0, /* a(n-4) */
          p1 = (seq_t)0, /* a(n-3) */
          p2 = (seq_t)1, /* a(n-2) */
          p3 = (seq_t)4; /* a(n-1) */
    seq_t v;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* a(i) = 4*a(i-1) -5*a(i-2) +2*a(i-3) +2*a(i-4) */
        v = 4 * p3 - 5 * p2 + 2 * p1 + 2 * p0;

        /* shift the window */
        p0 = p1;
        p1 = p2;
        p2 = p3;
        p3 = v;
    }

    return p3;
}
