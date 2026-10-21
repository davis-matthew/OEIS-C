/*
 * A260411  Number of ways n can be represented as a sum of a positive cube, a
 *          positive square, and a positive triangular number.
 * https://oeis.org/A260411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (c, s, t) with c>0, s>0, t>0 integers such that
 *            c^3 + s^2 + t(t+1)/2 = n. Computed by brute‑force enumeration of
 *            cubes and squares up to n
 * Range:     u8 n <= 11, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t int_sqrt_seq(seq_t x)
{
    seq_t l = 0, r = x, m, m2;
    while (l <= r)
    {
        m = (l + r) >> 1;
        m2 = m * m;
        if (m2 == x)
            return m;
        if (m2 < x)
            l = m + 1;
        else
            r = m - 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t sn = (seq_t)n;
    /* Enumerate cubes */
    for (idx_t c = 1; ; ++c)
    {
        seq_t cube = (seq_t)c * c * c;
        if (cube > sn)
            break;
        seq_t rem1 = sn - cube;   /* remaining after cube */
        /* Enumerate squares */
        for (idx_t s = 1; ; ++s)
        {
            seq_t square = (seq_t)s * s;
            if (square > rem1)
                break;
            seq_t rem = rem1 - square;  /* remaining for triangular part */
            if (rem == 0)
                continue; /* triangular number must be positive */
            /* Check whether rem is a triangular number:
               rem = t(t+1)/2  ⇔  8*rem + 1 is a perfect square. */
            seq_t tmp = rem * 8 + 1;
            seq_t d = int_sqrt_seq(tmp);
            if (d * d == tmp && d > 1)
                ++count;
        }
    }
    return count;
}
