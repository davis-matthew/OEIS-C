/*
 * A364251  a(n) = 1 if n is of the form q*(2^k), where q is one of the
 *          Mersenne primes (A000668) and k >= 0, otherwise a(n) = 0.
 * https://oeis.org/A364251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = q*2^k where q is a Mersenne prime (A000668) else
 *            0
 * Range:     u8 n <= 9502, u16 n <= 9817, u32 n <= 9817, u64 n <= 9817,
 *            u128 n <= 9817, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>    /* for size_t */

seq_t a(idx_t n)
{
    /* Remove all factors of 2: compute the odd part of n. */
    idx_t m = n;
    while ((m & 1ULL) == 0)
        m >>= 1;

    /* Mersenne primes that fit in a 64‑bit unsigned integer. */
    static const seq_t mersenne[] = {
        (seq_t)3ULL,
        (seq_t)7ULL,
        (seq_t)31ULL,
        (seq_t)127ULL,
        (seq_t)8191ULL,
        (seq_t)131071ULL,
        (seq_t)524287ULL,
        (seq_t)2147483647ULL,
        (seq_t)2305843009213693951ULL   /* 2^61 - 1 */
    };

    idx_t count = sizeof(mersenne) / sizeof(mersenne[0]);

    for (idx_t i = 0; i < count; ++i) {
        if ((seq_t)m == mersenne[i])
            return (seq_t)1;
    }
    return (seq_t)0;
}
