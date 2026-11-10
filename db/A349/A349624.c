/*
 * A349624  Dirichlet convolution of A326042 with A055615 (Dirichlet inverse
 *          of n), where A326042(n) = A064989(sigma(A003961(n))).
 * https://oeis.org/A349624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   multiplicative; a(p^e) = g(p^e) - p*g(p^(e-1)) where g =
 *            A326042, g(p^e) = A064989(sigma(nextprime(p)^e)), g(1)=1
 * Range:     i8 n <= 35, i16 n <= 1023, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 90 terms through n = 9896 (2026-11-10)
 * Helpers:   factor_u64, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static uint64_t A349624_prev_prime(uint64_t p)
{
    uint64_t q = p - 1;
    while (!is_prime_u64(q)) --q;
    return q;
}

/* A064989 applied to a uint64 value */
static seq_t A349624_A064989(uint64_t s)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    seq_t result = 1;
    if (s <= 1) return 1;
    factor_u64(s, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        seq_t b = (seq_t)A349624_prev_prime(pr[i]);
        for (uint64_t k = 0; k < ex[i]; ++k) result *= b;
    }
    return result;
}

/* A326042(p^e) */
static seq_t A349624_g(uint64_t p, uint64_t e)
{
    if (e == 0) return 1;
    uint64_t q = next_prime(p);
    uint64_t term = 1, s = 1;
    for (uint64_t k = 1; k <= e; ++k) {
        term *= q;
        s += term;
    }
    return A349624_A064989(s);
}

seq_t a(idx_t n)
{
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    seq_t result = 1;
    if (n < 1) return 0;
    if (n == 1) return 1;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        seq_t t = A349624_g(pr[i], ex[i]) - (seq_t)pr[i] * A349624_g(pr[i], ex[i] - 1);
        result *= t;
    }
    return result;
}
