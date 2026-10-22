/*
 * A349025  a(n) is multiplicative with a(p^e) = Sum_{d||e} p^(e-d), where
 *          d||e are the unitary divisors of e.
 * https://oeis.org/A349025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is multiplicative with a(p^e) = Sum_{d||e} p^(e-d)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"

/* Compute the value for a single prime power p^e */
static seq_t unitary_sum(seq_t p, idx_t e)
{
    /* Factor the exponent e */
    uint64_t efacs[64];
    uint64_t eexps[64];
    size_t ecnt = 0;
    factor_u64((uint64_t)e, efacs, eexps, &ecnt);

    seq_t sum = 0;
    /* Enumerate all unitary divisors of e (subsets of its prime factors) */
    size_t subsets = 1ULL << ecnt;
    for (size_t mask = 0; mask < subsets; ++mask) {
        idx_t div = 1;
        for (size_t i = 0; i < ecnt; ++i) {
            if (mask & (1ULL << i)) {
                for (idx_t k = 0; k < eexps[i]; ++k)
                    div *= efacs[i];
            }
        }
        sum += pow_seq(p, e - div);
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset originates at 1 */

    /* Factor the integer n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = unitary_sum((seq_t)primes[i], exps[i]);
        result *= term;
    }
    return result;
}
