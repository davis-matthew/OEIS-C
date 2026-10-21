/*
 * A252364  Number of (n+2)X(3+2) 0..3 arrays with every 3X3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 1 3 4 6 or 7.
 * https://oeis.org/A252364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 12*a(n-3) + 32*a(n-4) + 44*a(n-5) +
 *            37*a(n-6) - 32*a(n-7) - 81*a(n-8) - 12*a(n-9) + 47*a(n-10) +
 *            38*a(n-11) + 8*a(n-12) - 6*a(n-13) - 4*a(n-14) - 4*a(n-15) for n
 *            > 17 with initial values a(1) = 282, a(2) = 834, a(3) = 3054,
 *            a(4) = 10450, a(5) = 36078, a(6) = 124538, a(7)...
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constant initial values for n = 1..17 */
    static const seq_t init[17] = {
        (seq_t)282ULL,
        (seq_t)834ULL,
        (seq_t)3054ULL,
        (seq_t)10450ULL,
        (seq_t)36078ULL,
        (seq_t)124538ULL,
        (seq_t)427362ULL,
        (seq_t)1470006ULL,
        (seq_t)5070814ULL,
        (seq_t)17463766ULL,
        (seq_t)60088290ULL,
        (seq_t)206866218ULL,
        (seq_t)712422522ULL,
        (seq_t)2453110742ULL,
        (seq_t)8445738678ULL,
        (seq_t)29078502866ULL,
        (seq_t)100122283738ULL
    };

    /* recurrence coefficients */
    static const int coeff[15] = {
        1,   1,   12,  32,   44,  37,
       -32, -81, -12,  47,   38,  8,
        -6,  -4,  -4
    };

    if (n < 1)
        return (seq_t)0;  /* undefined below offset */

    if (n <= 17)
        return init[n - 1];

    /* buffer holds the last 15 terms: buffer[0] = a(k), buffer[1] = a(k-1), … , buffer[14] = a(k-14) */
    seq_t buffer[15];
    buffer[0] = init[16];           /* a(17) */
    for (int i = 1; i < 15; ++i)
        buffer[i] = init[16 - i];   /* a(16), a(15), … , a(3) */

    for (idx_t cur = 18; cur <= n; ++cur)
    {
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 15; ++k)
        {
            seq_t prod = buffer[k] *
                         (seq_t)(coeff[k] >= 0 ? coeff[k] : -coeff[k]);

            if (coeff[k] >= 0)
                pos += prod;
            else
                neg += prod;
        }
        seq_t val;
        if (pos >= neg)
            val = pos - neg;
        else
            val = 0; /* should not happen for valid recurrence */

        /* shift buffer to make val the newest term */
        for (int i = 14; i >= 1; --i)
            buffer[i] = buffer[i - 1];
        buffer[0] = val;
    }

    return buffer[0];
}
