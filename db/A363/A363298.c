/*
 * A363298  Number of refactorable unitary divisors of n.
 * https://oeis.org/A363298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, tau(d)|d, gcd(d,n/d)=1} 1
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts the unitary divisors d of n where the number of divisors τ(d)
   divides the value d. */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 1)
        return 0; /* undefined for n < 1, but return 0 for safety */

    uint64_t N = (uint64_t)n;
    /* factorize N */
    uint64_t primes[16];
    unsigned int exps[16];
    int cnt = 0;
    uint64_t temp = N;

    if (temp % 2 == 0) {
        unsigned int e = 0;
        while (temp % 2 == 0) { temp /= 2; ++e; }
        primes[cnt] = 2; exps[cnt] = e; ++cnt;
    }
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            primes[cnt] = p; exps[cnt] = e; ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp; exps[cnt] = 1; ++cnt;
    }

    uint64_t count = 0;
    /* enumerate all subsets of the prime factors (unitary divisors) */
    uint64_t total = (uint64_t)1 << cnt;
    for (uint64_t mask = 0; mask < total; ++mask) {
        uint64_t d = 1;
        uint64_t tau = 1;
        for (int i = 0; i < cnt; ++i) {
            if (mask & ((uint64_t)1 << i)) {
                /* multiply d by primes[i]^exps[i] */
                uint64_t p = primes[i];
                unsigned int e = exps[i];
                uint64_t pow = 1;
                for (unsigned int k = 0; k < e; ++k)
                    pow *= p;
                d *= pow;
                tau *= (e + 1);
            }
        }
        if (d % tau == 0)
            ++count;
    }

    return (seq_t)count;
}
