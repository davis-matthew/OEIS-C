/*
 * A348457  a(n) = Sum_{s=0..n} (-1)^s * ( Sum_{k=0..s} binomial(n,k) )^3.
 * https://oeis.org/A348457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{s=0..n} (-1)^s * ( Sum_{k=0..s} binomial(n,k) )^3
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 21, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t curC  = 1;           /* C(n,0) */
    seq_t sumS  = 1;           /* S(0) = C(n,0) */
    seq_t sign  = 1;           /* (-1)^s, starts with s=0 */

    for (idx_t s = 0; s <= n; ++s)
    {
        /* contribute (S(s))^3 with sign */
        seq_t val = sumS * sumS * sumS;
        if (sign > 0)
            result += val;
        else
            result -= val;

        /* prepare for next s */
        if (s < n)
        {
            /* C(n,s+1) = C(n,s) * (n-s) / (s+1) */
            seq_t numerator = curC * (seq_t)(n - s);
            seq_t denom     = (seq_t)(s + 1);
            curC = numerator / denom;
            sumS += curC;
            sign = -sign;        /* toggle (-1)^s */
        }
    }

    return result;
}
