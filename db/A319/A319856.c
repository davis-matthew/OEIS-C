/*
 * A319856  Maximum number that can be obtained by iteratively adding or
 *          multiplying together parts of the integer partition with Heinz
 *          number n until only one part remains.
 * https://oeis.org/A319856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum value obtained by repeatedly selecting two
 *            elements of the partition of n (by Heinz number) and replacing
 *            them with either their sum or product until only one element
 *            remains. This algorithm computes that maximum using dynamic
 *            programming over all subsets of the parts
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1; a(1) = 0. */
    if (n == 1) return (seq_t)0;

    /* Obtain the list of primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    /* Factor n.  The factor_u64 routine expects a uint64_t argument. */
    uint64_t pf[32], ex[32];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)n, pf, ex, &pf_cnt);

    /* Build the multiset of part indices (the indices of the primes).
       Each prime p corresponds to the index of p in the primes array. */
    seq_t parts[32];
    size_t m = 0;  /* number of parts */
    for (size_t i = 0; i < pf_cnt; ++i) {
        uint64_t p = pf[i];
        uint64_t e = ex[i];

        /* Find the index of prime p in the primes array. */
        size_t idx = 0;  /* 1‑based index of the prime */
        for (size_t j = 0; j < prime_cnt; ++j) {
            if (primes[j] == p) { idx = j + 1; break; }
        }

        /* Each occurrence of the prime contributes a part of value idx. */
        for (uint64_t k = 0; k < e; ++k) {
            parts[m++] = (seq_t)idx;
        }
    }
    free(primes);

    /* Dynamic programming over subsets of the parts. */
    const size_t states = 1U << m;
    seq_t *dp = (seq_t *)calloc(states, sizeof(seq_t));
    if (!dp) return (seq_t)0;  /* fallback on allocation failure */

    /* Base case: single‑element subsets. */
    for (size_t mask = 1; mask < states; ++mask) {
        if ((mask & (mask - 1)) == 0) {  /* only one bit set */
            size_t pos = 0;
            while (((mask >> pos) & 1U) == 0) ++pos;
            dp[mask] = parts[pos];
        }
    }

    /* Transition for larger subsets. */
    for (size_t mask = 1; mask < states; ++mask) {
        if ((mask & (mask - 1)) == 0) continue;  /* skip singletons */

        seq_t best = 0;
        /* Enumerate all non‑empty proper submasks `a` of `mask`. */
        for (size_t a = (mask - 1) & mask; a; a = (a - 1) & mask) {
            size_t b = mask ^ a;
            seq_t left  = dp[a];
            seq_t right = dp[b];

            seq_t sum = left + right;
            if (sum > best) best = sum;

            seq_t prod = left * right;
            if (prod > best) best = prod;
        }
        dp[mask] = best;
    }

    seq_t result = dp[states - 1];
    free(dp);
    return result;
}
