/*
 * A244571  Smallest prime p >= prime(n) such that p*prime(n)-2 is prime.
 * https://oeis.org/A244571
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p ≥ prime(n) such that p*prime(n)-2 is
 *            prime
 * Range:     u8 n <= 44, u16 n <= 6502, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;               /* undefined for n < 2, reference value */

    uint64_t pn = (uint64_t)get_prime(n);      /* nth prime, n ≥ 2 */
    uint64_t p = pn;                          /* current candidate prime */

    for (;;)
    {
        /* compute p * pn - 2, safe 128-bit multiplication */
        unsigned __int128 prod128 = (unsigned __int128)p * pn - 2;
        idx_t prod = (idx_t)prod128;          /* fits in idx_t for n ≤ 10000 */

        if (is_prime(prod))
            return (seq_t)p;                 /* found the required prime */

        p = next_prime(p + 1);                /* next prime > current */
    }
}
