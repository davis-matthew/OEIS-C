/*
 * A278254  Least number with the prime signature of n^2; square of the least
 *          number with the prime signature of n.
 * https://oeis.org/A278254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, pow_uint, seq_mul, sort_descending
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A046523(n)^2
 * where A046523(n) is the least number with the prime signature of n.
 *
 * We compute a(n) as the least number with the prime signature of n²,
 * which equals the square of the least number with the prime signature of n.
 *
 * Implementation steps:
 *   1. Factor n into primes and exponents using factor_u64.
 *   2. Sort the exponents in descending order.
 *   3. Multiply the corresponding smallest primes raised to those exponents.
 *   4. Square the resulting product.
 *
 * The function is O(k log k) with k = number of prime factors of n,
 * and works for any n >= 1 on the supported integer sizes.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Required helper functions provided by the build environment */
#include "../utils/factor_u64.h"      /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k); */
#include "../utils/pow_uint.h"       /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/seq_mul.h"        /* seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/sort_descending.h"

/* Sort an array of exponents in descending order (simple insertion sort) */

/* Sequence generator */
seq_t a(idx_t n)
{
    /* Handle the trivial case n = 0 if called (not part of the OEIS offset) */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Special case for n = 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Arrays to hold prime factors and exponents */
    uint64_t primes[64];
    uint64_t exps_u64[64];
    size_t factor_cnt = 0;

    /* Factor n */
    factor_u64((uint64_t)n, primes, exps_u64, &factor_cnt);
    if (factor_cnt == 0) {            /* this can only happen for n = 1, already handled */
        return (seq_t)1;
    }

    /* Convert exponents to int for sorting */
    int exps[64];
    for (size_t i = 0; i < factor_cnt; ++i) {
        exps[i] = (int)exps_u64[i];
    }

    /* Sort exponents descending so the largest exponent gets the smallest prime */
    sort_descending(exps, factor_cnt);

    /* Compute the least number with the same prime signature */
    seq_t prod = (seq_t)1;
    for (size_t i = 0; i < factor_cnt; ++i) {
        uint32_t prime = get_prime((idx_t)(i + 1));   /* primes are 2,3,5,... */
        seq_t term   = pow_uint((uint64_t)prime, (idx_t)exps[i]);
        prod         = seq_mul(prod, term);
    }

    /* Square the result to obtain a(n) = A046523(n)^2 */
    return seq_mul(prod, prod);
}
