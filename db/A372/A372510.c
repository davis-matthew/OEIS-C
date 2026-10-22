/*
 * A372510  Number of ordered factorizations of 2*n-1 into twin primes.
 * https://oeis.org/A372510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A372510: Number of ordered factorizations of 2*n-1 into twin primes.
   Each factor must itself be a twin prime.  If any prime factor of 2*n-1
   is not a twin prime, the value is 0.  The result equals the number
   of permutations of the multiset of prime factors of 2*n-1 when all
   factors are twin primes.  For example, 2*23-1 = 45 = 3^2·5, all
   twin primes ⇒ 3 ordered factorizations. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static bool is_twin_prime(uint64_t p) {
    if (p < 3) return false;
    if (p == 3) return true;              // 3 is twin (3,5)
    if ((p & 1) == 0) return false;       // even numbers >2 cannot be twin primes
    return is_prime((idx_t)(p - 2)) || is_prime((idx_t)(p + 2));
}

seq_t a(idx_t n) {
    /* 2*n-1 is the number to factor */
    uint64_t m = (uint64_t)((seq_t)n * 2 - 1);
    if (m == 1) return (seq_t)1;          // a(1) = 1

    /* arrays to hold prime factors and their exponents */
    uint64_t fac[32];
    unsigned exp[32];
    size_t cnt = 0;
    uint64_t tmp = m;

    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            uint64_t p = d;
            unsigned e = 0;
            do {
                ++e;
                tmp /= d;
            } while (tmp % d == 0);
            if (!is_twin_prime(p)) return (seq_t)0;
            fac[cnt] = p;
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) {                         // remaining prime factor
        if (!is_twin_prime(tmp)) return (seq_t)0;
        fac[cnt] = tmp;
        exp[cnt] = 1;
        ++cnt;
    }

    /* total exponent count */
    unsigned total = 0;
    for (size_t i = 0; i < cnt; ++i) total += exp[i];

    /* factorials up to total */
    seq_t fact[65];
    fact[0] = 1;
    for (unsigned i = 1; i <= total; ++i) fact[i] = fact[i - 1] * i;

    seq_t result = fact[total];
    for (size_t i = 0; i < cnt; ++i) result /= fact[exp[i]];

    return result;
}
