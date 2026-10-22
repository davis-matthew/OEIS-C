/*
 * A336697  a(n) = gcd((s+1)/2, 1+sigma(s)), where s is the n-th odd square,
 *          (2n-1)^2.
 * https://oeis.org/A336697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(((2n-1)^2 + 1)/2, 1 + sigma((2n-1)^2))
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 3781, u64 n <= 9767,
 *            u128 n <= 9767, big any n
 * Helpers:   gcd, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = gcd( (s+1)/2 , 1 + sum_of_divisors(s) ), with s = odd square (2n-1)^2 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/pow_uint.h"

static bool primes_built = false;
static uint64_t primes[20000];
static size_t prime_cnt = 0;

/* simple sieve to build primes up to 20000 */
static void build_primes(void) {
    if (primes_built) return;
    uint64_t limit = 20000;
    bool *is_composite = (bool *)malloc(limit + 1);
    for (uint64_t i = 2; i <= limit; ++i) is_composite[i] = false;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) is_composite[j] = true;
        }
    }
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
        }
    }
    free(is_composite);
    primes_built = true;
}

/* compute sigma(s) for s = (2n-1)^2 */
static seq_t sigma_of_odd_square(idx_t n) {
    build_primes();
    uint64_t m = (uint64_t)(2 * n - 1);          /* odd integer m */
    seq_t sigma = 1;
    uint64_t temp = m;
    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            /* contribution = (p^(2e+1)-1)/(p-1) */
            seq_t num = pow_uint(p, 2 * e + 1);
            seq_t denom = p - 1;
            seq_t contrib = (num - 1) / denom;
            sigma *= contrib;
        }
    }
    if (temp > 1) { /* remaining prime factor */
        uint64_t p = temp;
        seq_t num = pow_uint(p, 3);          /* 2*1+1 = 3 */
        seq_t denom = p - 1;
        seq_t contrib = (num - 1) / denom;
        sigma *= contrib;
    }
    return sigma;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* offset is 1, but harmless */
    /* compute s = (2n-1)^2 */
    seq_t m = (seq_t)(2 * n - 1);
    seq_t s = m * m;
    seq_t half = (s + 1) / 2;
    seq_t sigma_val = sigma_of_odd_square(n);
    seq_t one_plus_sigma = sigma_val + 1;
    return gcd(half, one_plus_sigma);
}
