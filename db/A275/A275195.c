/*
 * A275195  Sum of n-th powers of the roots of x^3 - 7*x^2 - 49*x - 49.
 * https://oeis.org/A275195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3, a(1) = 7, a(2) = 147; for n >= 3: a(n) = 7*a(n-1) +
 *            49*a(n-2) + 49*a(n-3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t mul_small(seq_t x, unsigned int m)
{
    seq_t r = 0;
    for (unsigned int i = 0; i < m; ++i)
        r += x;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)7;
    if (n == 2) return (seq_t)147;

    seq_t t0 = (seq_t)3;   /* a(0) */
    seq_t t1 = (seq_t)7;   /* a(1) */
    seq_t t2 = (seq_t)147; /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = mul_small(t2, 7);
        next += mul_small(t1, 49);
        next += mul_small(t0, 49);

        t0 = t1;
        t1 = t2;
        t2 = next;
    }
    return t2;
}
