/*
 * A367202  If n = Product(p_i^e_i), a(n) = Sum_{i = 1..k}(rad(n)/p_i)^e_i,
 *          where rad is A007947.
 * https://oeis.org/A367202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum over prime powers p^e||n of (rad(n)/p)^e, with a(1) =
 *            0
 * Range:     u8 n <= 55, u16 n <= 271, u32 n <= 2623, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 89 terms through n = 8439 (2026-11-01)
 * Helpers:   factor_u64, ipow, radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"
#include "../utils/radical.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1) return 0;

    /* Compute rad(n) */
    seq_t rad_val = radical(n);

    /* Factor n into prime powers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Accumulate the sum */
    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = rad_val / (seq_t)primes[i];
        seq_t term = ipow(base, (seq_t)exps[i]);
        sum += term;
    }
    return sum;
}
