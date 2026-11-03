/*
 * A359233  Number of divisors of 5*n-1 of form 5*k+1.
 * https://oeis.org/A359233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors of (5*n-1) congruent to 1 (mod 5)
 * Range:     u8 n <= 52, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9596 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Recursive helper: count divisors of the form 5*k+1 */
static void _count_divisors_of_form(const uint64_t *primes,
                                    const uint64_t *exps,
                                    size_t pf_cnt,
                                    size_t idx,
                                    seq_t cur,
                                    seq_t *cnt)
{
    if (idx == pf_cnt) {
        if ((cur % 5) == 1)
            ++(*cnt);
        return;
    }
    seq_t mult = 1;
    for (size_t e = 0; e <= exps[idx]; ++e) {
        _count_divisors_of_form(primes, exps, pf_cnt, idx + 1, cur * mult, cnt);
        mult *= (seq_t)primes[idx];
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* M = 5*n-1 */
    seq_t M = (seq_t)5 * n - 1;

    /* Factor M (fits in 64 bits for all n up to ~10⁶) */
    uint64_t primes[16];
    uint64_t exps[16];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)M, primes, exps, &pf_cnt);

    /* Count divisors ≡1 (mod 5) */
    seq_t count = 0;
    _count_divisors_of_form(primes, exps, pf_cnt, 0, 1, &count);
    return count;
}
