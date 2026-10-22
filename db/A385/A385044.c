/*
 * A385044  The number of unitary divisors of n that are 5-rough numbers
 *          (A007310).
 * https://oeis.org/A385044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = ∏_{p^e‖n} (p<=3 ? 1 : 2) = 2^{#distinct primes ≥5 in n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; value for 0 isn’t used */
        return (seq_t)0;

    uint64_t x = (uint64_t)n;
    unsigned cnt = 0;      /* number of distinct primes ≥5 */

    /* handle factor 2 (p<=3) – no counting needed */
    while ((x & 1ULL) == 0)          /* x % 2 == 0 */
        x >>= 1;                      /* divide by 2 */

    /* odd primes from 3 upwards */
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            if (p >= 5)
                ++cnt;
            /* remove all powers of p */
            while (x % p == 0)
                x /= p;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (x > 1 && x >= 5)
        ++cnt;

    /* compute 2^cnt */
    seq_t res = 1;
    for (unsigned i = 0; i < cnt; ++i)
        res <<= 1;

    return res;
}
