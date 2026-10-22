/*
 * A353481  a(n) = 1 if n is an odd squarefree semiprime, otherwise 0.
 * https://oeis.org/A353481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is an odd squarefree semiprime, otherwise 0
 * Range:     u8 n <= 9782, u16 n <= 9782, u32 n <= 9782, u64 n <= 9782,
 *            u128 n <= 9782, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if ((n & 1) == 0)            /* not odd */
        return 0;

    uint64_t m = (uint64_t)n;    /* work on 64‑bit integer */
    int prime_count = 0;         /* number of distinct odd prime factors */

    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            prime_count++;
            if (prime_count > 2)          /* more than two primes */
                return 0;
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
                if (exp > 1)               /* not squarefree */
                    return 0;
            }
        }
    }

    /* Remaining factor, if any, is an odd prime > sqrt(m) */
    if (m > 1) {
        prime_count++;
        if (prime_count > 2)
            return 0;
    }

    return (prime_count == 2) ? (seq_t)1 : (seq_t)0;
}
