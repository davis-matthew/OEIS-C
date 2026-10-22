/*
 * A369914  a(n) = n * Sum_{p|n, p prime} sopf(n/p) / p.
 * https://oeis.org/A369914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * Sum_{p|n, p prime} sopf(n/p) / p where sopf(m) is the
 *            sum of distinct prime factors of m
 * Range:     u8 n <= 33, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t sum_distinct_prime_factors(idx_t m)
{
    seq_t sum = 0;
    idx_t t = m;
    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            sum += p;
            while (t % p == 0)
                t /= p;
        }
    }
    if (t > 1)
        sum += t;
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t result = 0;

    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            idx_t m = n / p;
            seq_t sopf = sum_distinct_prime_factors(m);
            result += sopf * (seq_t)m;
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1) /* remaining prime divisor */
    {
        idx_t p = temp;
        idx_t m = n / p;
        seq_t sopf = sum_distinct_prime_factors(m);
        result += sopf * (seq_t)m;
    }

    return result;
}
