/*
 * A355742  Number of ways to choose a sequence of prime-power divisors, one
 *          of each prime index of n. Product of bigomega over the prime
 *          indices of n, with multiplicity.
 * https://oeis.org/A355742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e || n} (bigomega(π(p)))^e (totally multiplicative
 *            function; a(p_k) = bigomega(k))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, compute_pi, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/bigomega.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;          /* No prime factors → product = 1 */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);   /* primes[0..cnt-1] , exps[0..cnt-1] */

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Index of prime p : π(p) */
        seq_t pi_val = compute_pi(p);
        idx_t idx = (idx_t)pi_val;          /* idx fits in idx_t for n ≤ 10000 */

        /* bigomega(idx) is A001222(idx) */
        seq_t base = bigomega(idx);

        /* base^e */
        seq_t pow = 1;
        for (uint64_t j = 0; j < e; ++j) pow *= base;

        result *= pow;
    }
    return result;
}
