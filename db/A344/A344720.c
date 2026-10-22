/*
 * A344720  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)^2.
 * https://oeis.org/A344720
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)^2
 * Range:     u8 n <= 17, u16 n <= 282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safe guard – the sequence is defined for n ≥ 1 */
    unsigned long long sum_pos = 0;   /* sum of terms with odd k (positive sign) */
    unsigned long long sum_neg = 0;   /* sum of terms with even k (negative sign) */

    for (idx_t k = 1; k <= n; ++k) {
        idx_t q = n / k;                 /* floor(n/k) */
        unsigned long long term = (unsigned long long)q;
        term *= term;                    /* square it */
        if (k & (idx_t)1)                /* odd k → positive sign */
            sum_pos += term;
        else                              /* even k → negative sign */
            sum_neg += term;
    }

    unsigned long long res = sum_pos - sum_neg;   /* sum_pos > sum_neg, so result is positive */
    return (seq_t)res;
}
