/*
 * A280246  a(n) = Product_{d|n} A023896(d), where A023896(m) is the sum of
 *          totatives of m.
 * https://oeis.org/A280246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n} A023896(d), where A023896(m) = sum of
 *            totatives of m
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper to compute Euler's totient function for a 64‑bit integer */
static seq_t my_phi(idx_t m)
{
    seq_t result = (seq_t)m;
    idx_t temp   = m;
    idx_t p      = 2;

    for (; (seq_t)p * (seq_t)p <= (seq_t)temp; ++p)
    {
        if (temp % p == 0)
        {
            while (temp % p == 0)  temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)            /* last prime factor > sqrt(m) */
        result -= result / temp;

    return result;
}

/* Sum of totatives of m (A023896) */
static seq_t sum_totatives(idx_t m)
{
    if (m == (idx_t)1)
        return (seq_t)1;
    seq_t phi_val = my_phi(m);
    return (seq_t)m * phi_val / 2;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* undefined for n = 0, return 0 as a safe guard */
        return (seq_t)0;

    seq_t result = (seq_t)1;
    idx_t i;

    for (i = (idx_t)1; i * i <= n; ++i)
    {
        if (n % i == (idx_t)0)
        {
            result = result * sum_totatives(i);
            if (i != n / i)
                result = result * sum_totatives(n / i);
        }
    }
    return result;
}
