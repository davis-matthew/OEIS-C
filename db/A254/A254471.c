/*
 * A254471  Sixth partial sums of fifth powers (A000584).
 * https://oeis.org/A254471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(1 + n)*(2 + n)*(3 + n)*(4 + n)*(5 + n)*(6 + n)* * (-29
 *            + 54*n + 81*n^2 + 24*n^3 + 2*n^4)/665280
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 20, u64 n <= 176, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Do nothing for indices below the offset (n < 1). */
    if (n == 0) return 0;

    /* Denominator factors of 665280 = 2^6 * 3^3 * 5 * 7 * 11 */
    int denom[12] = {2,2,2,2,2,2,3,3,3,5,7,11};
    int rem = 12;                       /* number of remaining factors */

    seq_t res = 1;

    /* Multiply the seven consecutive numbers */
    for (idx_t i = 0; i < 7; ++i)
    {
        seq_t v = (seq_t)(n + i);

        /* Cancel any remaining denominator factors that divide v */
        for (int j = 0; j < rem; )
        {
            int f = denom[j];
            if (v % f == 0)
            {
                v /= f;
                denom[j] = denom[rem - 1];
                --rem;
            }
            else
            {
                ++j;
            }
        }

        /* Multiply into the result */
        res *= v;

        /* After each multiplication cancel factors from the result itself */
        for (int j = 0; j < rem; )
        {
            int f = denom[j];
            if (res % f == 0)
            {
                res /= f;
                denom[j] = denom[rem - 1];
                --rem;
            }
            else
            {
                ++j;
            }
        }
    }

    /* Compute the polynomial part: 2*n^4 + 24*n^3 + 81*n^2 + 54*n - 29 */
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n3 * nn;

    seq_t poly = 2 * n4;
    poly += 24 * n3;
    poly += 81 * n2;
    poly += 54 * nn;
    poly -= 29;   /* For n >= 1 the subtrahend never underflows */

    /* Cancel remaining denominator factors with the polynomial */
    seq_t v = poly;
    for (int j = 0; j < rem; )
    {
        int f = denom[j];
        if (v % f == 0)
        {
            v /= f;
            denom[j] = denom[rem - 1];
            --rem;
        }
        else
        {
            ++j;
        }
    }

    /* Final multiplication and any last cancellations */
    res *= v;
    for (int j = 0; j < rem; )
    {
        int f = denom[j];
        if (res % f == 0)
        {
            res /= f;
            denom[j] = denom[rem - 1];
            --rem;
        }
        else
        {
            ++j;
        }
    }

    /* At this point rem should be zero; res is a(n) */
    return res;
}
