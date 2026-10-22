/*
 * A392012  The sum of the exponential divisors of the cubefree numbers.
 * https://oeis.org/A392012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of exponential divisors of the n-th cubefree number *
 *            (i.e. a(n) = σₑ(A004709(n)), where σₑ(n) = product_{p^e||n} * (
 *            sum_{d|e} p^d ) )
 * Range:     u8 n <= 143, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/build_primes_upto.h"
#include "../utils/int_cube_root.h"

/* Compute the sum of exponential divisors of a cubefree integer m.
 * For cubefree numbers the prime exponents are 1 or 2,
 * so only the cases e=1 (term = p) and e=2 (term = p + p²) occur.   */
static seq_t sigma_exp(uint64_t m)
{
    if (m <= 1) return (seq_t)1;
    seq_t result = 1;
    uint64_t temp = m;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            seq_t term;
            if (exp == 1) {
                term = (seq_t)p;
            } else {            /* exp == 2 for cubefree numbers */
                seq_t pp = (seq_t)p * (seq_t)p;
                term = (seq_t)p + pp;
            }
            result *= term;
        }
    }
    if (temp > 1) { /* remaining prime factor with exponent 1 */
        result *= (seq_t)temp;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 1. Find the n‑th cubefree integer.  */
    size_t limit = (size_t)(n * 2 + 1000);          /* ample buffer  */
    idx_t cube_root = int_cube_root((idx_t)limit);

    /* Build primes up to cube_root. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(cube_root, &primes, &prime_cnt);

    /* Boolean array marking cube‑free numbers.  */
    bool *cube_free = (bool *)malloc((size_t)(limit + 1));
    memset(cube_free, 1, limit + 1);                /* initial true */

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t cube = p * p * p;
        for (uint64_t j = cube; j <= limit; j += cube) {
            cube_free[j] = false;                   /* divisible by a cube */
        }
    }

    /* Locate the n‑th cube‑free integer. */
    idx_t count = 0;
    uint64_t target = 1;
    for (uint64_t m = 1; m <= limit; ++m) {
        if (cube_free[m]) {
            ++count;
            if (count == n) { target = m; break; }
        }
    }

    /* Clean up. */
    free(cube_free);
    free(primes);

    /* 2. Return σₑ(target).  */
    return sigma_exp(target);
}
