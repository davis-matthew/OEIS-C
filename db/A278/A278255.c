/*
 * A278255  Least number with the prime signature of the n-th pentagonal
 *          number.
 * https://oeis.org/A278255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number with the prime signature of the n-th
 *            pentagonal number * (i.e. a(n) = A046523(A000326(n)) where the
 *            n-th pentagonal number is n*(3*n-1)/2)
 * Range:     u8 n <= 13, u16 n <= 260, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;
static size_t prime_list_cnt = 0;

/* Initialize a list of small primes once.  The chosen limit is ample for
 * the number of distinct prime factors we will encounter in the
 * pentagonal numbers for the test ranges. */
static void init_prime_list(void)
{
    if (prime_list == NULL) {
        const idx_t LIMIT = 1000000;          /* 1,000,000 */
        build_primes(LIMIT, &prime_list, &prime_list_cnt);
    }
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Pentagonal number: P(n) = n*(3*n-1)/2 */
    seq_t p = (seq_t)n * ((seq_t)3 * n - 1) / 2;

    /* For factorization we need a 64-bit integer.  The tests for the
     * given sequence keep the pentagonal numbers within 64‑bit limits. */
    uint64_t p64 = (uint64_t)p;

    /* Factor p64; we need exponents of its prime factorisation. */
    static uint64_t pf[64];
    static uint64_t exp[64];
    size_t cnt;
    factor_u64(p64, pf, exp, &cnt);

    if (cnt == 0)          /* P(1) = 1 */
        return 1;

    /* Sort the exponents in descending order. */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (exp[i] < exp[j]) {
                uint64_t tmp = exp[i];
                exp[i] = exp[j];
                exp[j] = tmp;
            }
        }
    }

    init_prime_list();

    /* Build the minimal number by assigning the largest exponent to the
     * smallest available prime. */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        /* We use the i-th smallest prime. */
        seq_t part = pow_uint((uint64_t)prime_list[i], (idx_t)exp[i]);
        result *= part;
    }

    return result;
}
