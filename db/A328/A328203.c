/*
 * A328203  Expansion of Sum_{k>=1} k * x^k / (1 - x^(2*k))^2.
 * https://oeis.org/A328203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = 2*a(n); for odd n: a(n) = (n*d(n)+sigma(n))/2
 * Range:     u8 n <= 74, u16 n <= 5354, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 81 terms through n = 9053 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void div_sigma(idx_t n, seq_t *divs, seq_t *sigma)
{
    idx_t remaining = n;
    seq_t d = 1;
    seq_t s = 1;
    for (idx_t p = 2; p * p <= remaining; ++p)
    {
        if (remaining % p == 0)
        {
            int cnt = 0;
            seq_t pow = 1;
            seq_t sum = 1;
            while (remaining % p == 0)
            {
                remaining /= p;
                ++cnt;
                pow *= (seq_t)p;
                sum += pow;
            }
            d *= (seq_t)(cnt + 1);
            s *= sum;
        }
    }
    if (remaining > 1)
    {
        d *= 2;
        s *= (1 + (seq_t)remaining);
    }
    *divs = d;
    *sigma = s;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return 2 * a(n >> 1);

    seq_t d, sigma_val;
    div_sigma(n, &d, &sigma_val);
    return ((seq_t)n * d + sigma_val) / 2;
}
