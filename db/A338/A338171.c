/*
 * A338171  a(n) is the sum of those divisors d of n such that tau(d) divides
 *          sigma(d).
 * https://oeis.org/A338171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum_{d|n, tau(d)|sigma(d)} d
 * Range:     u8 n <= 131, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sigma_u64(idx_t d)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= d / i; ++i)
    {
        if (d % i == 0)
        {
            sum += (seq_t)i;
            idx_t j = d / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum;
}

static idx_t tau_u64(idx_t d)
{
    idx_t cnt = 0;
    for (idx_t i = 1; i <= d / i; ++i)
    {
        if (d % i == 0)
        {
            cnt += 1;
            idx_t j = d / i;
            if (j != i)
                cnt += 1;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            seq_t sigma1 = sigma_u64(d1);
            idx_t tau1 = tau_u64(d1);
            if ((sigma1 % (seq_t)tau1) == 0)
                result += (seq_t)d1;

            idx_t d2 = n / i;
            if (d2 != d1)
            {
                seq_t sigma2 = sigma_u64(d2);
                idx_t tau2 = tau_u64(d2);
                if ((sigma2 % (seq_t)tau2) == 0)
                    result += (seq_t)d2;
            }
        }
    }
    return result;
}
