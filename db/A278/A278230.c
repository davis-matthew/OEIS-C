/*
 * A278230  Least number with the prime signature of 2*prime(n) + 1.
 * https://oeis.org/A278230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number with the same prime signature as
 *            (2*prime(n)+1)
 * Range:     u8 n <= 32, u16 n <= 3830, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/factor_u64.h"
#include "../utils/pow_int.h"

/* Smallest primes used when reassigning exponents.  The list is long enough
   for any number that can appear (2*prime(n)+1) <= 2*10^6 tests. */
static const uint32_t SMALL_PRIMES[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};

seq_t a(idx_t n)
{
    /* Step 1: Compute 2*prime(n)+1 */
    uint32_t p = get_prime(n);            /* nth prime as uint32_t */
    seq_t m = (seq_t)2 * (seq_t)p + 1;    /* target number, fits in uint64_t */

    /* Step 2: Factor m.  We only need prime exponents. */
    uint64_t primes[20], exps[20];
    size_t cnt = 0;
    factor_u64((uint64_t)m, primes, exps, &cnt);

    /* Step 3: Sort exponents in descending order. */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (exps[j] > exps[i]) {
                uint64_t tmp = exps[i];
                exps[i] = exps[j];
                exps[j] = tmp;
            }
        }
    }

    /* Step 4: Recombine with the smallest primes to get the minimal number. */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t power = pow_int((seq_t)SMALL_PRIMES[i], (idx_t)exps[i]);
        result = result * power;
    }

    return result;
}
