/*
 * A328390  Maximal digit value in primorial base expansion of the arithmetic
 *          derivative of n: a(n) = A328114(A003415(n)).
 * https://oeis.org/A328390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A328114(A003415(n))
 * Range:     u8 n <= 1039, u16 n <= 8425, u32 n <= 8425, u64 n <= 8425,
 *            u128 n <= 8425, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): maximal digit in the primorial base expansion of the arithmetic
   derivative of n. */
#include <stdint.h>
#include <stddef.h>

static const uint64_t small_primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL,
    17ULL, 19ULL, 23ULL, 29ULL, 31ULL, 37ULL,
    41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL,
    67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL,
    97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL
};
static const size_t small_prime_count = sizeof(small_primes) / sizeof(small_primes[0]);

/* seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* 1. Compute arithmetic derivative of n. */
    uint64_t N = (uint64_t)n;           /* original n as an integer */
    uint64_t tmp = N;
    seq_t der = 0;

    for (size_t i = 0; i < small_prime_count && small_primes[i] * small_primes[i] <= tmp; ++i) {
        uint64_t p = small_primes[i];
        uint64_t e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            e++;
        }
        if (e)
            der += ((seq_t)N / p) * (seq_t)e;
    }
    if (tmp > 1) {                     /* remaining prime factor */
        der += ((seq_t)N / tmp) * (seq_t)1;
    }

    /* 2. Find maximal digit in primorial base expansion of the derivative. */
    seq_t value = der;
    seq_t max_dig = 0;
    seq_t weights[32];                 /* enough for small derivatives (~10^5) */
    seq_t weight = 1;
    size_t idx = 0;

    /* Build weights for ascending primes until weight > value. */
    while (weight <= value && idx < sizeof(weights)/sizeof(weights[0])) {
        weights[idx++] = weight;
        /* Next weight is previous weight multiplied by current prime. */
        if (idx < sizeof(weights)/sizeof(weights[0]))
            weight *= (seq_t)small_primes[idx - 1];
    }

    /* Process digits from largest weight to smallest. */
    for (size_t i = idx; i-- > 0;) {
        seq_t d = value / weights[i];
        if (d > max_dig)
            max_dig = d;
        value %= weights[i];
    }

    return max_dig;
}
