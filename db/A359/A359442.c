/*
 * A359442  a(n) = Sum_{d|n} d^(n + 1 - d - n/d).
 * https://oeis.org/A359442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n + 1 - d - n/d)
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 20, u64 n <= 32, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 557 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

/* Recursive helper to generate all divisors of n from its prime factorisation */
static void build_divisors(const uint64_t *pr, const uint64_t *ex,
                           size_t cnt, int pos, uint64_t cur,
                           uint64_t *divs, size_t *dcnt)
{
    if (pos == (int)cnt) {
        divs[(*dcnt)++] = cur;
        return;
    }
    uint64_t p = pr[pos];
    uint64_t val = 1;
    for (int i = 0; i <= (int)ex[pos]; ++i) {
        build_divisors(pr, ex, cnt, pos + 1, cur * val, divs, dcnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; this value is unused */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    /* factor_n returns the prime factors of n in `primes` with exponents in `exps` */
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    uint64_t divs[256];
    size_t dcnt = 0;
    build_divisors(primes, exps, pcnt, 0, 1, divs, &dcnt);

    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divs[i];
        idx_t e = n + 1 - d - (n / d);          /* exponent is non‑negative */
        seq_t term = ipow((seq_t)d, (seq_t)e);
        sum += term;
    }
    return sum;
}
