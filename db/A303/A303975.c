/*
 * A303975  Number of distinct prime factors in the product of prime indices
 *          of n.
 * https://oeis.org/A303975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = A001221(A156061(n))
 * Range:     u8 n <= 8067, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1) {       /* n = 0 or 1 has no prime factorisation */
        return 0;
    }

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64], exps[64];
    size_t pcnt;
    factor_u64(nn, primes, exps, &pcnt);   /* distinct prime divisors of n */

    uint64_t uniq[64];
    size_t ucnt = 0;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];

        /* index of prime p: pi(p) */
        seq_t pi_seq = compute_pi(p);
        uint64_t idx = (uint64_t)pi_seq;          /* idx fits in uint64_t for required bounds */

        if (idx <= 1) continue;   /* index 1 contributes nothing to the product */

        uint64_t idx_pr[64], idx_ex[64];
        size_t icnt;
        factor_u64(idx, idx_pr, idx_ex, &icnt);   /* prime factors of the index */

        for (size_t j = 0; j < icnt; ++j) {
            uint64_t q = idx_pr[j];

            /* add to the set of unique prime factors of all indices */
            bool present = false;
            for (size_t k = 0; k < ucnt; ++k) {
                if (uniq[k] == q) { present = true; break; }
            }
            if (!present) uniq[ucnt++] = q;
        }
    }

    return (seq_t)ucnt;
}
