/*
 * A284587  Sum of the divisors of n that are not divisible by 13.
 * https://oeis.org/A284587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p==13 ? 1 : (p^(e+1)-1)/(p-1))
 * Range:     u8 n <= 16, u16 n <= 673, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9313 (2026-11-01)
 * Helpers:   factor_u64, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = sum of the divisors of n that are not divisible by 13.  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* sequence defined for n ≥ 1; guard for safety */
        return (seq_t)0;

    if (n == 1)  /* only divisor is 1, which is not divisible by 13 */
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t term;
        if (p == 13ULL) {
            term = (seq_t)1;
        } else {
            seq_t power = ipow_u64_idx(p, e + 1);
            term = (power - (seq_t)1) / ((seq_t)(p - 1));
        }
        result *= term;
    }

    return result;
}
