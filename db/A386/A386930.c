/*
 * A386930  Number of divisors d of n such that (-d)^d == -d^d (mod n).
 * https://oeis.org/A386930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that * (-d)^d == -d^d (mod
 *            n). * For odd d this always holds. * For even d the condition is
 *            equivalent to n | 2*d^d
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* modular exponentiation (base^exp mod mod), all arguments are
 * unsigned 64-bit integers.  For the small values occurring here
 * (n ≤ 10000) 64‑bit arithmetic is sufficient.
 */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* small primes up to sqrt(10000) = 100; there are 25 of them. */
static const uint64_t SMALL_PRIMES[25] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
    71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
};

seq_t a(idx_t n)
{
    /* n is always ≥ 1 */
    uint64_t NN = n;                  /* uint64_t used for arithmetic */
    /* factor n into prime powers */
    uint64_t prime[30];
    unsigned  exp[30];
    size_t    pcnt = 0;

    uint64_t tmp = NN;
    for (size_t i = 0; i < 25 && SMALL_PRIMES[i] * SMALL_PRIMES[i] <= tmp; ++i) {
        uint64_t p = SMALL_PRIMES[i];
        if (tmp % p == 0) {
            unsigned e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            prime[pcnt]  = p;
            exp[pcnt]    = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {                   /* remaining prime factor */
        prime[pcnt]  = tmp;
        exp[pcnt]    = 1;
        ++pcnt;
    }

    /* enumerate all divisors; we need at most 128 for n ≤ 10000 */
    uint64_t divisors[256];
    size_t dcnt = 1;
    divisors[0] = 1ULL;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = prime[i];
        unsigned e = exp[i];
        size_t old = dcnt;
        uint64_t mult = 1ULL;
        for (unsigned k = 1; k <= e; ++k) {
            mult *= p;
            for (size_t j = 0; j < old; ++j) {
                divisors[dcnt++] = divisors[j] * mult;
            }
        }
    }

    /* count divisors satisfying the congruence */
    seq_t count = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        if (d & 1ULL) {                 /* odd divisor – always qualifies */
            ++count;
        } else {                        /* even divisor – test n | 2*d^d */
            uint64_t pow = mod_pow_u64(d, d, NN);
            uint64_t twice = 2ULL * pow;
            if (twice % NN == 0ULL)
                ++count;
        }
    }
    return count;
}
