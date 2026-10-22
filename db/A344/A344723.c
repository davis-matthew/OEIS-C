/*
 * A344723  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)^5.
 * https://oeis.org/A344723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor(n/k)^5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7172,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard against 0 */
    seq_t pos = 0;                  /* sum of terms with odd k (positive sign)   */
    seq_t neg = 0;                  /* sum of terms with even k (negative sign)  */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t q   = n / k;                /* floor(n/k) */
        seq_t x   = (seq_t)q;
        seq_t x2  = x * x;                /* x^2        */
        seq_t x4  = x2 * x2;              /* x^4        */
        seq_t term = x4 * x;              /* x^5        */
        if ((k & 1) == 1)                /* k odd  -> +   */
            pos += term;
        else                               /* k even -> -   */
            neg += term;
    }
    return pos - neg;                /* pos > neg for all n >= 1 */
}
