/*
 * A244800  Number of moduli m such that (prime(n) mod m) is even, where 1 <=
 *          m < prime(n).
 * https://oeis.org/A244800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of m (1 ≤ m ≤ prime(n)) such that prime(n) mod m
 *            is even
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset < 1 */

    /* obtain the n‑th prime */
    uint64_t p = (uint64_t)get_prime(n);

    uint64_t count = 0;
    uint64_t m = 1;

    /* parity of the prime */
    int p_even = (p & 1ULL) == 0;

    while (m <= p)
    {
        uint64_t q = p / m;      /* floor(p/m) */
        uint64_t r = p / q;      /* last m with this q */

        if (p_even)
        {
            if (q & 1ULL)        /* q odd → need m even */
            {
                uint64_t evens = (r / 2ULL) - ((m - 1ULL) / 2ULL);
                count += evens;
            }
            else                 /* q even → all m satisfy */
            {
                count += (r - m + 1ULL);
            }
        }
        else
        {
            if (q & 1ULL)        /* q odd → need m odd */
            {
                uint64_t odds = ((r + 1ULL) / 2ULL) - (m / 2ULL);
                count += odds;
            }
            /* q even → none */
        }

        m = r + 1ULL;
    }

    return (seq_t)count;
}
