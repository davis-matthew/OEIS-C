/*
 * A386777  a(n) = n^2*sigma_6(n).
 * https://oeis.org/A386777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^2 * sigma_6(n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 255, u128 n <= 2748,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t _pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        exp >>= 1U;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t sigma = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }
            seq_t numer = _pow_seq((seq_t)p, 6 * (e + 1));
            numer -= 1;
            seq_t denom = _pow_seq((seq_t)p, 6);
            denom -= 1;
            sigma *= numer / denom;
        }
    }
    if (temp > 1)
    {
        seq_t numer = _pow_seq((seq_t)temp, 12);
        numer -= 1;
        seq_t denom = _pow_seq((seq_t)temp, 6);
        denom -= 1;
        sigma *= numer / denom;
    }
    seq_t nsq = (seq_t)n * (seq_t)n;
    return nsq * sigma;
}
