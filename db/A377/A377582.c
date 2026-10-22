/*
 * A377582  Expansion of e.g.f. (1 + x * exp(x))^3.
 * https://oeis.org/A377582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} k^(n-k) * binomial(3,k)/(n-k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simplified using the fact that binomial(3,k)=0 for k>3:          */
/* a(n) = 3n + 3 * 2^(n-2) * n(n-1) + 3^(n-3) * n(n-1)(n-2) for n≥0 */

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = 0;

    /* k = 1 term: 3 * n */
    res += (seq_t)3 * (seq_t)n;

    if (n >= 2)
    {
        /* k = 2 term: 3 * 2^(n-2) * n * (n-1) */
        seq_t pow2 = pow_seq((seq_t)2, n - 2);
        seq_t term = (seq_t)3 * pow2 * (seq_t)n * (seq_t)(n - 1);
        res += term;
    }

    if (n >= 3)
    {
        /* k = 3 term: 3^(n-3) * n * (n-1) * (n-2) */
        seq_t pow3 = pow_seq((seq_t)3, n - 3);
        seq_t term = pow3 * (seq_t)n * (seq_t)(n - 1) * (seq_t)(n - 2);
        res += term;
    }

    return res;
}
