/*
 * A341837  If n = Product (p_j^k_j) then a(n) = Product ((-1)^k_j *
 *          binomial(n, k_j)).
 * https://oeis.org/A341837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ∏_{p^k||n} ((-1)^k * binomial(n, k))
 * Range:     i8 n <= 11, i16 n <= 23, i32 n <= 95, i64 n <= 767,
 *            i128 n <= 8191, big any n
 * Validated: standard, pass, 61 terms through n = 9632 (2026-11-01)
 * Helpers:   binomial, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* a(1) is the first term; undefined for n<1 */

    /* factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        idx_t k = (idx_t)exps[i];
        seq_t term = binomial(n, k);
        if (k & 1) /* (-1)^k */
            term = -term;
        result = result * term;
    }
    return result;
}
