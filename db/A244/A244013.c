/*
 * A244013  Denominators of rational approximations to sqrt(7) obtained from
 *          Newton's method.
 * https://oeis.org/A244013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the n-th Newton approximation of sqrt(7).
 *            Recurrence: p₀ = 2, q₀ = 1; For k ≥ 0: p_{k+1} = p_k² + 7·q_k²
 *            q_{k+1} = 2·p_k·q_k Then a(n) = q_n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t p = (seq_t)2;
    seq_t q = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t p_next = p * p + (seq_t)7 * q * q;
        seq_t q_next = (seq_t)2 * p * q;
        p = p_next;
        q = q_next;
    }
    return q;
}
