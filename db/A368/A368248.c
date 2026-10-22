/*
 * A368248  The number of unitary divisors of the cubefull part of n
 *          (A360540).
 * https://oeis.org/A368248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 2^k, where k = number of distinct primes with exponent ≥
 *            3 * in the prime factorisation of n (i.e., in the cubefull part
 *            of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work in native integer for factorisation; result in seq_t. */
    unsigned long long m = (unsigned long long)n;
    unsigned long long cnt = 0;   /* count of primes with exponent ≥ 3 */
    unsigned long long e;
    unsigned long long x;

    /* Handle factor 2 */
    if (m > 1)
    {
        e = 0;
        while ((m & 1ULL) == 0ULL)
        {
            e++;
            m >>= 1;
        }
        if (e >= 3)
            cnt++;
    }

    /* Handle odd factors */
    for (x = 3ULL; x * x <= m; x += 2ULL)
    {
        if (m % x == 0ULL)
        {
            e = 0;
            while (m % x == 0ULL)
            {
                e++;
                m /= x;
            }
            if (e >= 3)
                cnt++;
        }
    }

    /* Remaining prime factor (if any) has exponent 1 < 3, so ignore */
    /* Compute 2^cnt in seq_t */
    seq_t result = 1;
    while (cnt)
    {
        result *= 2;
        cnt--;
    }
    return result;
}
