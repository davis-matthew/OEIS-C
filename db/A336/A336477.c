/*
 * A336477  a(n) = 1 if a regular n-gon is constructible with ruler (or, more
 *          precisely, an unmarked straightedge) and compass, 0 otherwise.
 * https://oeis.org/A336477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 iff n is of the form 2^k × (product of distinct Fermat
 *            primes)
 * Range:     u8 n <= 8010, u16 n <= 8010, u32 n <= 8010, u64 n <= 8010,
 *            u128 n <= 8010, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* domain is n ≥ 1, but handle 0 safely */

    uint64_t x = (uint64_t)n;

    /* Remove all factors of 2 – they are always allowed. */
    while ((x & 1ULL) == 0)
        x >>= 1;

    /* Examine odd prime factors. */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            if (exp > 1)
                return (seq_t)0;   /* repeated odd prime factor → not constructible */

            /* p must be a Fermat prime: p−1 has to be a power of two. */
            uint64_t m = p - 1;
            if ((m & (m - 1)) != 0)
                return (seq_t)0;   /* p−1 not a power of two → not a Fermat prime */
        }
    }

    /* Remaining factor (if any) is a prime greater than sqrt(original n). */
    if (x > 1) {
        uint64_t m = x - 1;
        if ((m & (m - 1)) != 0)
            return (seq_t)0;       /* not a Fermat prime */
    }

    return (seq_t)1;               /* all checks passed */
}
