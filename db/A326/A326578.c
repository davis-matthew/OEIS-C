/*
 * A326578  a(n) =
 *          n^2*denominator(n*Bernoulli(n-1))/denominator(Bernoulli(n-1)) =
 *          n*A326478(n).
 * https://oeis.org/A326578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 / gcd(n, lcm{p | p-1 divides n-1})
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* for n ≥ 1.  This is equivalent to the definition in the OEIS
   entry for A326578.  The implementation factors n and removes from
   n^2 any prime factor p that also divides the denominator of
   B_{n-1}.  For odd n-1>1 the Bernoulli number is zero, so the
   denominator is 1 and the result is n^2. */

seq_t a(idx_t n)
{
    if (n == 0)           /* offset starts at 1, but guard anyway */
        return (seq_t)0;

    if (n == 1)
        return (seq_t)1;

    /* n-1 is the index of the Bernoulli number */
    idx_t k = n - 1;       /* k = n-1 */

    /* If k is odd and >1, B_k = 0 so denom = 1 and a(n)=n^2 */
    if ((k & 1U) && k > 1U)
        return (seq_t)n * (seq_t)n;

    idx_t temp = n;                  /* remaining factor to process */
    seq_t g = (seq_t)1;              /* product of primes that divide both n and the denom */

    /* Handle prime factor 2 separately (p-1=1 divides any k) */
    if (temp % 2U == 0) {
        g *= (seq_t)2U;
        while (temp % 2U == 0)
            temp /= 2U;
    }

    /* Process odd prime factors */
    for (idx_t p = 3U; p <= temp / p; p += 2U) {
        if (temp % p == 0) {
            if (k % (p - 1U) == 0U)
                g *= (seq_t)p;
            while (temp % p == 0)
                temp /= p;
        }
    }

    /* If a prime factor larger than sqrt remains. */
    if (temp > 1U) {
        if (k % (temp - 1U) == 0U)
            g *= (seq_t)temp;
    }

    /* Final result: a(n) = n^2 / g  (g always divides n^2) */
    seq_t nsq = (seq_t)n * (seq_t)n;
    return nsq / g;
}
