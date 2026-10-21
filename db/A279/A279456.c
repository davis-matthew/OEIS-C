/*
 * A279456  Numbers k such that number of distinct primes dividing k is odd
 *          and number of prime divisors (counted with multiplicity) of k is
 *          even.
 * https://oeis.org/A279456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A279456: Numbers k such that the number of distinct prime divisors of k
 * (omega(k)) is odd and the number of prime divisors with multiplicity
 * (bigomega(k)) is even.
 *
 * The function searches for the nth such number by brute‑force enumeration.
 * For each candidate k it counts its distinct and total prime factors using
 * simple trial division.  When the parity conditions are satisfied, the
 * counter is increased; once the desired index is reached, k is returned
 * (cast to seq_t).
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t k = 2;               /* 1 is not in the sequence */

    while (1)
    {
        uint64_t tmp = k;
        int distinct = 0;
        int total = 0;

        for (uint64_t p = 2; p * p <= tmp; ++p)
        {
            if (tmp % p == 0)
            {
                ++distinct;
                while (tmp % p == 0)
                {
                    tmp /= p;
                    ++total;
                }
            }
        }
        if (tmp > 1)              /* last prime factor > sqrt(k) */
        {
            ++distinct;
            ++total;
        }

        if ((distinct & 1) && !(total & 1))
        {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
}
