/*
 * A349626  Möbius transform of A326042, where A326042(n) =
 *          A064989(sigma(A003961(n))).
 * https://oeis.org/A349626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} A008683(n/d)*A326042(d); multiplicative, a(p^e)
 *            = f(p^e) - f(p^(e-1)), f = A326042, f(p^e) =
 *            A064989(sigma(nextprime(p)^e))
 * Range:     i8 n <= 35, i16 n <= 1023, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 109 terms through n = 9593 (2026-11-10)
 * Helpers:   factor_u64, is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

static uint64_t A349626_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    for (;; --p)
        if (is_prime((idx_t)p)) return p;
}

/* A064989 applied to m */
static seq_t A349626_A064989(uint64_t m)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    seq_t result = 1;
    if (m <= 1) return 1;
    factor_u64(m, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (pr[i] == 2) continue;
        seq_t b = (seq_t)A349626_prev_prime(pr[i]);
        for (uint64_t k = 0; k < ex[i]; ++k) result *= b;
    }
    return result;
}

/* A326042(p^e) */
static seq_t A349626_f(uint64_t p, uint64_t e)
{
    if (e == 0) return 1;
    uint64_t q = next_prime(p);
    uint64_t s = 1, t = 1;
    for (uint64_t k = 0; k < e; ++k) {
        t *= q;
        s += t;
    }
    return A349626_A064989(s);
}

seq_t a(idx_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    seq_t result = 1;
    if (n < 1) return 0;
    if (n == 1) return 1;
    factor_u64((uint64_t)n, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        result *= (A349626_f(pr[i], ex[i]) - A349626_f(pr[i], ex[i] - 1));
    }
    return result;
}
