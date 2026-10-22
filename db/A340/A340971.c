/*
 * A340971  a(n) = Sum_{k=0..n} n^k * binomial(n,k) * binomial(2*k,k).
 * https://oeis.org/A340971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=0..n} n^k * binomial(n,k) * binomial(2*k,k)
 *
 * Computes the nth term of OEIS sequence A340971 using a straightforward
 * multiplicative recurrence.  The function works for any integer type
 * supplied for seq_t and idx_t.  Intermediate results are computed in
 * seq_t, relying on its support for the usual arithmetic operators.
 */
seq_t a(idx_t n)
{
    seq_t power = 1;   /* n^0 */
    seq_t comb  = 1;   /* binomial(n,0) */
    seq_t cc    = 1;   /* binomial(0,0)   */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        sum += power * comb * cc;

        if (k == n)
            break;

        power *= (seq_t)n;

        /* Update binomial(n,k+1) = binomial(n,k) * (n-k)/(k+1) */
        comb *= (seq_t)(n - k);
        comb /= (k + 1);

        /* Update binomial(2*(k+1),(k+1)) = binomial(2k,k) * (4*(k+1)-2)/(k+1) */
        cc *= (seq_t)(4 * (k + 1) - 2);
        cc /= (k + 1);
    }

    return sum;
}
