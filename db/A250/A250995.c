/*
 * A250995  Number of (1+1) X (n+1) 0..2 arrays with nondecreasing
 *          min(x(i,j),x(i,j-1)) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 12*a(n-3) + 15*a(n-4) + 11*a(n-5) -
 *            5*a(n-6) - 3*a(n-7) Initial values: a(1)=40, a(2)=169, a(3)=646,
 *            a(4)=2359, a(5)=8298, a(6)=28450, a(7)=95628
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The recurrence is linear of order 7; we only need the last seven terms. */
    static const seq_t init[7] = { 40, 169, 646, 2359, 8298, 28450, 95628 };

    if (n <= 7u)
        return init[n - 1u];

    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)
        buf[i] = init[i];

    for (idx_t k = 8; k <= n; ++k)
    {
        /* compute a(k) = 6*a(k-1) -7*a(k-2) -12*a(k-3) +15*a(k-4)
                         + 11*a(k-5) -5*a(k-6) -3*a(k-7) */
        seq_t pos = 6 * buf[6] + 15 * buf[3] + 11 * buf[2];
        seq_t neg = 7 * buf[5] + 12 * buf[4] + 5 * buf[1] + 3 * buf[0];
        seq_t next = pos - neg;   /* pos > neg for all n >= 8 */

        /* shift buffer left: buf[0]=buf[1], ..., buf[5]=buf[6] */
        for (idx_t j = 0; j < 6; ++j)
            buf[j] = buf[j + 1];
        buf[6] = next;
    }
    return buf[6];
}
