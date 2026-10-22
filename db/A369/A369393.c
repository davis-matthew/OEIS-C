/*
 * A369393  a(n) = 2^n*(((n + 1)/2)^n - n!).
 * https://oeis.org/A369393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^n - 2^n * n!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    /* Exponentiation by squaring */
    seq_t result = (seq_t)1;
    while (exp)
    {
        if (exp & (idx_t)1u)
            result *= base;
        exp >>= (idx_t)1u;
        if (exp)
            base *= base;
    }
    return result;
}

static seq_t fact_seq(idx_t n)
{
    seq_t f = (seq_t)1;
    for (idx_t i = (idx_t)2u; i <= n; ++i)
        f *= (seq_t)i;
    return f;
}

seq_t a(idx_t n)
{
    /* (n+1)^n */
    seq_t pow_n1 = pow_seq((seq_t)n + (seq_t)1u, n);

    /* 2^n */
    seq_t pow2 = pow_seq((seq_t)2u, n);

    /* n! */
    seq_t fac = fact_seq(n);

    /* 2^n * n! */
    seq_t term = pow2 * fac;

    /* a(n) = (n+1)^n - 2^n * n! */
    return pow_n1 - term;
}
