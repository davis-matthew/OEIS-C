/*
 * A363969  Expansion of Sum_{k>0} k^2 * x^(2*k-1) / (1 - x^(2*k-1)).
 * https://oeis.org/A363969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d odd} ((d+1)/2)^2
 * Range:     u8 n <= 30, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   sumDivisorsRec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/sumDivisorsRec.h"

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset starts at 1, but guard for 0 */

    /* Remove all factors of 2 – even divisors are irrelevant. */
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;

    /* Factorise the odd part m */
    uint64_t primes[64];
    uint8_t exps[64];
    size_t num = 0;
    idx_t tmp = m;

    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            uint8_t e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            primes[num] = (uint64_t)p;
            exps[num] = e;
            ++num;
        }
    }
    if (tmp > 1) {          /* remaining prime factor */
        primes[num] = (uint64_t)tmp;
        exps[num] = 1;
        ++num;
    }

    seq_t sum = 0;
    sumDivisorsRec(0, (idx_t)1, primes, exps, num, &sum);
    return sum;
}
