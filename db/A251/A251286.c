/*
 * A251286  Number of (n+1) X (2+1) 0..1 arrays with every 2 X 2 subblock
 *          having a single 1 or two 1s on the same edge or main diagonally.
 * https://oeis.org/A251286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - 9*a(n-3) - 7*a(n-4) + 7*a(n-5)
 *            Initial values: a(1)=22, a(2)=71, a(3)=219, a(4)=701, a(5)=2193,
 *            a(6)=6980
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base values for n = 1..6 */
    static const seq_t base[6] = {22, 71, 219, 701, 2193, 6980};

    if (n <= 6)
        return base[n-1];

    /* a1..a5 hold the previous five terms, a1 = a(n-5), a5 = a(n-1) */
    seq_t a1 = base[1];  /* a(2) */
    seq_t a2 = base[2];  /* a(3) */
    seq_t a3 = base[3];  /* a(4) */
    seq_t a4 = base[4];  /* a(5) */
    seq_t a5 = base[5];  /* a(6) */
    seq_t ai = a5;

    for (idx_t k = 7; k <= n; ++k)
    {
        ai = 2*a5 + 7*a4;   /* first part of the recurrence */
        ai += 7*a1;         /* +7*a(n-5) */
        ai -= 9*a3;         /* -9*a(n-3) */
        ai -= 7*a2;         /* -7*a(n-4) */
        /* shift the window for the next iteration */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = ai;
    }
    return ai;
}
