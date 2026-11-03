/*
 * A343253  a(n) is the least k0 <= n such that v_11(n), the 11-adic order of
 *          n, can be obtained by the formula: v_11(n) = log_11(n / L_11(k0,
 *          n)), where L_11(k0, n) is the lowest common denominator of the
 *          elements of the set S_11(k0, n) = {(1/n)*binomial(n, k), with 0 <
 *          k <= k0 such that k is not divisible...
 * https://oeis.org/A343253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest power of a prime different from 11 dividing n
 *            (equivalently, the greatest power of a prime ≠ 11 that divides
 *            n) For example: a(12) = 4 (since 12 = 2²·3 and 2² is the largest
 *            such power)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 80 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

/* Return the a(n) value as defined above. */
seq_t a(idx_t n) {
    if (n <= 1) {   /* n = 1 has no prime factors ≠ 11 */
        return (seq_t)1;
    }

    /* Factor n: primes[] contains distinct prime factors, exps[] the exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t best = (seq_t)1;          /* at least 1 */

    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 11) {      /* ignore 11 */
            continue;
        }
        seq_t term = ipow_u64_idx(primes[i], (idx_t)exps[i]);   /* p^e */
        if (cmp_seq_t(&term, &best) > 0) {
            best = term;
        }
    }
    return best;
}
