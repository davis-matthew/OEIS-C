/*
 * A363853  Number of divisors of 7*n-6 of form 7*k+1.
 * https://oeis.org/A363853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A279061(7*n-6) // number of divisors of 7*n-6 that are 1
 *            (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive helper: count divisors of the partially built product
   whose current value modulo 7 equals cur_mod.  'idx' iterates over
   distinct prime factors. */
static void
count_mod1_rec (size_t idx, unsigned int cur_mod,
                const uint64_t *primes, const uint64_t *exps,
                size_t cnt, seq_t *out)
{
    if (idx == cnt) {                /* all prime factors processed */
        if (cur_mod == 1)
            (*out)++;                 /* divisor satisfies d mod 7 == 1 */
        return;
    }

    /* Process prime primes[idx] raised to all exponents 0 .. exps[idx] */
    const uint64_t p = primes[idx];
    const unsigned int pmod = p % 7u;

    /* Exponent 0: keep cur_mod unchanged */
    count_mod1_rec (idx + 1, cur_mod, primes, exps, cnt, out);

    /* Exponents >= 1 */
    unsigned int m = cur_mod;
    for (uint64_t e = 1; e <= exps[idx]; ++e) {
        m = (m * pmod) % 7u;          /* multiply by p^e modulo 7 */
        count_mod1_rec (idx + 1, m, primes, exps, cnt, out);
    }
}

/* Sequence function: a(n) = number of divisors of 7*n-6 that are 1 (mod 7). */
seq_t
a (idx_t n)
{
    /* Compute target number: 7*n-6.  In 64‑bit arithmetic the product
       may overflow for very large n; we assume the test harness uses
       n values that keep 7*n-6 within 64 bits. */
    uint64_t m = (uint64_t)n * 7ull - 6ull;   /* target number */

    /* Factorise m into primes^exponents.  factor_u64 stores up to 64 factors. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64 (m, primes, exps, &cnt);

    seq_t result = 0;
    count_mod1_rec (0, 1u, primes, exps, cnt, &result);
    return result;
}
