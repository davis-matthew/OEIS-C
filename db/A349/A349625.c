/*
 * A349625  Dirichlet convolution of A000027 (identity function) with the
 *          Dirichlet inverse of A326042, where A326042(n) =
 *          A064989(sigma(A003961(n))).
 * https://oeis.org/A349625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} d*A349623(n/d); multiplicative, computed on
 *            prime powers via the Dirichlet inverse g of
 *            f(n)=A064989(sigma(A003961(n)))
 * Range:     i8 n <= 35, i16 n <= 1023, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 95 terms through n = 8830 (2026-11-10)
 * Helpers:   factor_u64, is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static uint64_t A064989_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    while (!is_prime_u64(p)) --p;
    return p;
}

/* f(p^e) = A064989(sigma(nextprime(p)^e)) */
static seq_t A326042_pp(uint64_t p, unsigned e)
{
    uint64_t q = next_prime(p);
    uint64_t s = 1, t = 1;
    for (unsigned i = 1; i <= e; i++) {
        t *= q;
        s += t;
    }
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(s, pr, ex, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; i++) {
        if (pr[i] == 2) continue;
        seq_t b = (seq_t)A064989_prev_prime(pr[i]);
        for (uint64_t k = 0; k < ex[i]; k++) res *= b;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; i++) {
        uint64_t p = pr[i];
        unsigned E = (unsigned)ex[i];
        seq_t *f = mem_new(seq_t, E + 1);
        seq_t *g = mem_new(seq_t, E + 1);
        f[0] = 1;
        g[0] = 1;
        for (unsigned j = 1; j <= E; j++) f[j] = A326042_pp(p, j);
        for (unsigned k = 1; k <= E; k++) {
            seq_t s = 0;
            for (unsigned j = 1; j <= k; j++) s += f[j] * g[k - j];
            g[k] = -s;
        }
        /* a(p^E) = sum_{j=0..E} p^j * g(p^(E-j)) */
        seq_t val = 0;
        seq_t pw = 1;
        for (unsigned j = 0; j <= E; j++) {
            val += pw * g[E - j];
            pw *= (seq_t)p;
        }
        mem_free(f);
        mem_free(g);
        result *= val;
    }
    return result;
}
