/*
 * A364252  a(n) = 1 if n has no other prime factors than 2 and/or Mersenne
 *          primes, otherwise 0.
 * https://oeis.org/A364252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 9543, u16 n <= 9543, u32 n <= 9543, u64 n <= 9543,
 *            u128 n <= 9543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: 1 if all prime factors of n are 2 or Mersenne primes,
   0 otherwise.  We factor n repeatedly by each known Mersenne prime
   (and 2).  Any remaining factor > 1 implies a forbidden prime. */

#include <stddef.h>
#include <stdint.h>

/* Mersenne primes that fit in 64 bits, together with the prime 2. */
static const uint64_t allowed_primes[] = {
    2ULL,          /* prime 2 */
    3ULL,          /* 2^2 - 1  */
    7ULL,          /* 2^3 - 1  */
    31ULL,         /* 2^5 - 1  */
    127ULL,        /* 2^7 - 1  */
    8191ULL,       /* 2^13 - 1 */
    131071ULL,     /* 2^17 - 1 */
    524287ULL,     /* 2^19 - 1 */
    2147483647ULL, /* 2^31 - 1 */
    2305843009213693951ULL /* 2^61 - 1 */
};
static const size_t allowed_count =
        sizeof(allowed_primes) / sizeof(allowed_primes[0]);

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; guard against 0 input */
        return (seq_t)0;

    uint64_t v = (uint64_t)n;

    for (size_t i = 0; i < allowed_count; ++i) {
        uint64_t p = allowed_primes[i];
        if (p > v) break;          /* no need to try larger primes */
        while (v % p == 0) {
            v /= p;
            if (v == 1)
                return (seq_t)1;
        }
    }

    /* If any factor > 1 remains, it is not an allowed prime. */
    return (seq_t)(v == 1 ? 1 : 0);
}
