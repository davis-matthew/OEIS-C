/*
 * A376554  Square root of largest unitary square divisor of binomial(n,
 *          floor(n/2)).
 * https://oeis.org/A376554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(product_{p|C(n,floor(n/2)), v_p(C(n,floor(n/2)))
 *            even} p^{v_p(C(n,floor(n/2)) / 2})
 * Range:     u8 n <= 129, u16 n <= 362, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"

/* cached list of primes up to the greatest n seen so far */
static uint64_t *cached_primes = NULL;
static size_t cached_prime_count = 0;
static idx_t cached_prime_limit = 0;

static idx_t exp_in_fact(idx_t n, idx_t p) {
    idx_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;  /* C(0,0)=1 */

    idx_t k = n >> 1;  /* floor(n/2) */

    /* update cached prime list if needed */
    if (n > cached_prime_limit) {
        if (cached_primes) free(cached_primes);
        build_primes_upto(n, &cached_primes, &cached_prime_count);
        cached_prime_limit = n;
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cached_prime_count; ++i) {
        idx_t p = cached_primes[i];
        idx_t e = exp_in_fact(n, p) - exp_in_fact(k, p) - exp_in_fact(n - k, p);
        if (e > 0 && !(e & 1u)) {          /* e even and positive */
            seq_t mul = seq_pow((seq_t)p, e >> 1);  /* p^(e/2) */
            result *= mul;
        }
    }
    return result;
}
