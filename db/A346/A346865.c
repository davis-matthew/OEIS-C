/*
 * A346865  Sum of divisors of the n-th hexagonal number.
 * https://oeis.org/A346865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n*(2*n-1))
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(m) = sum of all positive divisors of m */

/* Computes sigma(m) for 64‑bit inputs; results fit in seq_t */
static seq_t sigma_uint64(uint64_t m)
{
    seq_t result = 1;
    uint64_t temp  = m;
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            seq_t pk   = 1;   /* p^k */
            seq_t sum  = 1;   /* 1 + p + p^2 + ... + p^k */
            while (temp % p == 0)
            {
                temp /= p;
                pk *= p;
                sum += pk;
            }
            result *= sum;
        }
    }
    /* If a prime factor remains */
    if (temp > 1)
        result *= (seq_t)(1 + temp);
    return result;
}

seq_t a(idx_t n)
{
    /* Assume n >= 1, but guard against an accidental 0 */
    if (n == 0)
        return 0;

    uint64_t n64 = n;
    uint64_t hex1 = n64;               /* factor n */
    uint64_t hex2 = 2 * n64 - 1;       /* factor (2n-1), coprime with n */

    seq_t sigma1 = sigma_uint64(hex1);
    seq_t sigma2 = sigma_uint64(hex2);

    return sigma1 * sigma2;
}
