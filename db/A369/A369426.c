/*
 * A369426  The number of unitary divisors of n that are of the form p^p,
 *          where p is a prime.
 * https://oeis.org/A369426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of unitary divisors of n that are of the form p^p,
 *            where p is a prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count unitary prime‑power divisors p^p of n. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* Working copy of n. */
    uint64_t tmp = (uint64_t)n;

    seq_t count = 0;
    uint64_t e;

    /* Handle factor 2 separately. */
    if ((tmp & 1ULL) == 0) {
        e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++e;
        }
        if (e == 2)
            ++count;
    }

    /* Even powers other than 2 cannot be of the form p^p. */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (e == p)
                ++count;
        }
    }

    /* Remaining prime factor > sqrt(original n) has exponent 1,
       which can never equal the prime itself. */
    return count;
}
