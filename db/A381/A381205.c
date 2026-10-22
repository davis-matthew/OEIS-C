/*
 * A381205  a(n) is the cardinality of the set of bases and exponents
 *          (including exponents = 1) in the prime factorization of n.
 * https://oeis.org/A381205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the number of distinct integers that are either a prime factor
 *            of n or an exponent in the prime factorization of n (including
 *            exponent 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0; /* a(1) = 0 by convention */

    /* Maximum # of distinct primes in 64‑bit n is < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    uint64_t uniq[128];
    size_t uniqcnt = 0;

    for (size_t i = 0; i < cnt; ++i) {
        /* insert prime base */
        uint64_t val = primes[i];
        bool found = false;
        for (size_t j = 0; j < uniqcnt; ++j) {
            if (uniq[j] == val) { found = true; break; }
        }
        if (!found) uniq[uniqcnt++] = val;

        /* insert exponent */
        val = exps[i];
        found = false;
        for (size_t j = 0; j < uniqcnt; ++j) {
            if (uniq[j] == val) { found = true; break; }
        }
        if (!found) uniq[uniqcnt++] = val;
    }

    return (seq_t) uniqcnt;
}
