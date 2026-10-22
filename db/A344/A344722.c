/*
 * A344722  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)^4.
 * https://oeis.org/A344722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)^4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, though offset starts at 1 */

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t q = (seq_t)(n / k);          /* floor(n/k) */
        seq_t q2 = q * q;                  /* q^2 */
        seq_t q4 = q2 * q2;                /* q^4 */
        if (k & 1U)                        /* odd k -> positive sign */
            sum += q4;
        else                               /* even k -> negative sign */
            sum -= q4;
    }
    return sum;
}
