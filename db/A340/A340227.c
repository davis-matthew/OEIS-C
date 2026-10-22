/*
 * A340227  Number of pairs of divisors of n, (d1,d2), such that d1 < d2 and
 *          d1*d2 is squarefree.
 * https://oeis.org/A340227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^ω(n) - 1)/2 * where ω(n) is the number of distinct
 *            prime divisors of n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

/* Count distinct prime factors of n (n >= 1). */
static idx_t distinct_prime_count(uint64_t n)
{
    idx_t cnt = 0;
    if (n % 2ULL == 0ULL) {
        ++cnt;
        while (n % 2ULL == 0ULL)
            n /= 2ULL;
    }
    for (uint64_t p = 3ULL; p <= n / p; p += 2ULL) {
        if (n % p == 0ULL) {
            ++cnt;
            while (n % p == 0ULL)
                n /= p;
        }
    }
    if (n > 1ULL)
        ++cnt;
    return cnt;
}

seq_t a(idx_t n)
{
    /* Compute ω(n). */
    idx_t k = distinct_prime_count((uint64_t)n);

    /* Compute 3^k. */
    seq_t power = 1;
    for (idx_t i = 0; i < k; ++i)
        power = power * (seq_t)3;

    /* a(n) = (3^k - 1)/2. */
    power -= (seq_t)1;
    power /= (seq_t)2;
    return power;
}
