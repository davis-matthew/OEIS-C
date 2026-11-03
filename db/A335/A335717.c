/*
 * A335717  a(n) is the smallest dividend m of the Euclidean division m = d*n
 *          + r such that m/d = r/n.
 * https://oeis.org/A335717
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n⁴ / k + 2·n² + k, where k is the largest divisor of n³
 *            that is < n
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 55 terms through n = 8639 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive search for the largest divisor of n³ that is < n. */
static void gen_best_rec(const uint64_t *pr, const int *exp, size_t idx, size_t cnt,
                         seq_t cur, seq_t n_val, seq_t *best)
{
    if (idx == cnt) {
        if (cur < n_val && cur > *best)
            *best = cur;
        return;
    }
    seq_t v = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        gen_best_rec(pr, exp, idx + 1, cnt, cur * v, n_val, best);
        v = v * (seq_t)pr[idx];
    }
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined for n < 2, safe fallback */

    seq_t n_val = (seq_t)n;
    seq_t n2 = n_val * n_val;
    seq_t n4 = n2 * n2;

    /* Factor n to get primes and exponents. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Exponents for n³. */
    int exp3[20];
    for (size_t i = 0; i < cnt; ++i)
        exp3[i] = (int)(3 * exps[i]);

    /* Find the largest divisor k < n. */
    seq_t best_k = 1; /* 1 is always a divisor */
    gen_best_rec(primes, exp3, 0, cnt, (seq_t)1, n_val, &best_k);

    /* Compute a(n). */
    seq_t m = n4 / best_k + 2 * n2 + best_k;
    return m;
}
