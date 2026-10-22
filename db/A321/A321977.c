/*
 * A321977  8-dimensional Catalan numbers.
 * https://oeis.org/A321977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0!*1!*...*7! * (8*n)! / ( n!*(n+1)!*...*(n+7)! )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   build_primes_upto, factorial, legendre_fact, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility function prototypes (provided by the build environment). */
#include "../utils/build_primes_upto.h"
#include "../utils/factorial.h"
#include "../utils/seq_mul.h"
#include "../utils/pow_uint.h"
#include "../utils/legendre_fact.h"

/* Cache for the constant 0!*1!*...*7!. */
static seq_t C = 0;
static bool C_initialized = false;

/* Prime table for all primes up to 8*n. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    /* Ensure the constant C is computed once. */
    if (!C_initialized) {
        C = 1;                 /* seq_t literal 1 is safe for all integer types */
        for (idx_t k = 0; k <= 7; ++k) {
            seq_t fk = factorial(k);
            C = seq_mul(C, fk);
        }
        C_initialized = true;
    }

    /* Quick results for n = 0, 1 (should be 1). */
    if (n == 0 || n == 1)
        return 1;

    idx_t m = n * 8;                 /* upper factorial: (8*n)! */
    ensure_primes(m);

    seq_t result = 1;                /* start from multiplicative identity */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > m)
            break;

        /* Exponent from the constant C = 0!*1!*...*7!. */
        idx_t exp = 0;
        for (int k = 0; k <= 7; ++k)
            exp += legendre_fact(k, p);

        /* Add exponent from (8*n)! . */
        exp += legendre_fact(m, p);

        /* Subtract exponents from (n)! ... (n+7)! . */
        for (int k = 0; k <= 7; ++k)
            exp -= legendre_fact(n + k, p);

        if (exp > 0) {
            seq_t powp = pow_uint(p, exp);
            result = seq_mul(result, powp);
        }
    }

    return result;
}
