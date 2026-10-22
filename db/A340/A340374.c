/*
 * A340374  a(n) = 1 if the odd part of n satisfies Korselt's criterion (is in
 *          A324050), 0 otherwise.
 * https://oeis.org/A340374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the odd part of n satisfies Korselt's criterion
 *            (square‑free and (p-1)|(n-1) for every prime divisor p), 0
 *            otherwise
 * Range:     u8 n <= 551, u16 n <= 9491, u32 n <= 9491, u64 n <= 9491,
 *            u128 n <= 9491, big any n
 * Helpers:   is_prime, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/odd_part.h"
#include "../utils/is_prime.h"

static bool satisfies_korselt(uint64_t m)
{
    if (m <= 1)                      /* 1 is counted as satisfying */
        return true;
    if (is_prime(m))                 /* primes always satisfy */
        return true;

    uint64_t temp = m;
    uint64_t m_minus_1 = m - 1;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
                if (cnt > 1)          /* not square‑free */
                    return false;
            }
            if (m_minus_1 % (p - 1) != 0)
                return false;        /* Korselt divisibility fails */
        }
    }

    if (temp > 1) {                   /* remaining prime factor */
        if (m_minus_1 % (temp - 1) != 0)
            return false;
    }

    return true;                     /* all conditions satisfied */
}

seq_t a(idx_t n)
{
    seq_t odd = odd_part((seq_t)n);
    uint64_t m = (uint64_t)odd;       /* odd part fits in 64 bits */
    return satisfies_korselt(m) ? (seq_t)1 : (seq_t)0;
}
