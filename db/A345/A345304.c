/*
 * A345304  a(n) = Sum_{p|n, p prime} p * lcm(p,n/p).
 * https://oeis.org/A345304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p * lcm(p, n/p)
 * Range:     u8 n <= 16, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as defined above.
 * For each distinct prime factor p of n:
 *   If its exponent e == 1: add n * p
 *   Else (e >= 2): add n
 * Thus a(n) = n * (sum of primes with exponent 1 + count of primes with exponent >=2)
 */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t N = (seq_t)n;
    uint64_t temp = n;
    seq_t sum_p_e1   = 0;   /* sum of distinct primes whose exponent is 1 */
    seq_t count_ge2 = 0;   /* number of distinct primes whose exponent >= 2 */

    while (temp > 1) {
        /* Find the smallest prime divisor of temp. */
        uint64_t p;
        if ((temp & 1ULL) == 0)           /* divisible by 2? */
            p = 2;
        else {
            p = 0;
            for (uint64_t i = 3; i <= temp / i; i += 2) {
                if (temp % i == 0) { p = i; break; }
            }
            if (p == 0)                  /* temp itself is prime */
                p = temp;
        }

        /* Count the exponent of p in temp. */
        uint64_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }

        /* Accumulate according to the exponent. */
        if (exp == 1)
            sum_p_e1 += (seq_t)p;
        else
            count_ge2 += 1;
    }

    return N * (sum_p_e1 + count_ge2);
}
