/*
 * A384452  a(n) is the sum of squares of the unitary divisors of n!.
 * https://oeis.org/A384452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p ≤ n, p prime} (p^(2·exp_p(n!)) + 1) where exp_p(n!)
 *            = Σ_{k≥1} ⌊n / p^k⌋
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t primes_limit = 0;

    if (n > primes_limit) {
        free(primes);
        build_primes_upto(n, &primes, &prime_count);
        primes_limit = n;
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        idx_t exp = 0;
        idx_t m = n;
        while (m >= p) {
            m /= p;
            exp += m;
        }
        seq_t exp2 = (seq_t)exp * (seq_t)2;
        seq_t term = big_pow((seq_t)p, exp2);
        term += (seq_t)1;
        result *= term;
    }

    return result;
}
