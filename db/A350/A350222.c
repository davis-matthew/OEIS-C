/*
 * A350222  a(n) = Sum_{k=1..n} (-1)^(k+1) * floor((n/k)^3).
 * https://oeis.org/A350222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(k+1) * floor((n/k)^3)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    seq_t sn  = (seq_t)n;          /* convert index to seq_t once */
    seq_t n3  = sn * sn * sn;      /* n^3, computed in seq_t */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t sk  = (seq_t)k;
        seq_t k3  = sk * sk * sk;  /* k^3, computed in seq_t */
        seq_t term = n3 / k3;      /* floor((n/k)^3) */

        if (k & 1U)               /* k odd -> add term   */
            ans += term;
        else                      /* k even -> subtract  */
            ans -= term;
    }
    return ans;
}
