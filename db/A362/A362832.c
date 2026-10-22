/*
 * A362832  Number of partitions of n into two distinct parts (s,t) such that
 *          phi(s) = phi(t).
 * https://oeis.org/A362832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor((n-1)/2)} [phi(k)=phi(n-k)]
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute Euler's totient function for 0 <= n <= 10^4.
 * Uses trial division, fast enough for n <= 10^4. */
static uint64_t phi_uint64(idx_t n)
{
    if (n == 0)
        return 0;
    uint64_t result = (uint64_t)n;
    uint64_t m = (uint64_t)n;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
    }
    if (m > 1)
        result -= result / m;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t count = 0;
    idx_t limit = (n - 1) / 2;          /* floor((n-1)/2) */
    for (idx_t k = 1; k <= limit; ++k)
    {
        uint64_t phi_k = phi_uint64(k);
        uint64_t phi_t = phi_uint64(n - k);
        if (phi_k == phi_t)
            ++count;                   /* count is of type seq_t */
    }
    return count;
}
