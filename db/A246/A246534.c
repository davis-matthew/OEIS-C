/*
 * A246534  a(n) = Sum_{k=1..n} 2^(T(k)-1), where T(k)=k(k+1)/2 = A000217(k).
 * https://oeis.org/A246534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} 2^(T(k)-1), with T(k)=k(k+1)/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0)         /* a(0) = 0 */
        return sum;

    seq_t term  = (seq_t)1; /* 2^(T(1)-1) = 2^0 = 1 */
    seq_t pow2k = (seq_t)2; /* 2^1, will be updated to 2^k */

    for (idx_t k = 1; k <= n; ++k)
    {
        sum += term;          /* add 2^(T(k)-1) */

        if (k < n)
        {
            /* multiplier = 2^(k+1) = pow2k * 2 */
            seq_t mul = pow2k * (seq_t)2;

            /* term_{k+1} = term_k * 2^(k+1) */
            term  = term * mul;
            pow2k = mul;      /* pow2k becomes 2^(k+1) for next loop */
        }
    }

    return sum;
}
