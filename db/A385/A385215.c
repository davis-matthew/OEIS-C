/*
 * A385215  Number of maximal sparse submultisets of the prime indices of n,
 *          where a multiset is sparse iff 1 is not a first difference.
 * https://oeis.org/A385215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of maximal sparse submultisets of the prime
 *            indices of n
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

/* Precomputed counts of maximal independent sets for a path of length L
   (vertices labelled by consecutive indices present in the multiset). */
static seq_t mis_cache[65];
static bool   mis_cache_init = false;

static seq_t mis_count_of_len(size_t len)
{
    if (!mis_cache_init) {
        mis_cache_init = true;
        mis_cache[0] = 1;          /* unused but harmless */
        mis_cache[1] = 1;
        mis_cache[2] = 2;
        for (size_t i = 3; i < sizeof(mis_cache)/sizeof(mis_cache[0]); ++i)
            mis_cache[i] = mis_cache[i-2] + mis_cache[i-3];
    }
    return mis_cache[len];
}

seq_t a(idx_t n)
{
    /* n = 1 -> empty multiset → only the empty set is maximal */
    if (n <= 1) return (seq_t)1;

    /* Factor n (distinct primes only) */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t   cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    if (cnt == 0) return (seq_t)1;      /* Safety: for n = 1 */

    /* Convert each prime factor to its 1‑based prime index */
    idx_t  indices[64];
    size_t num_idx = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        seq_t    pi = prime_pi(p);    /* pi(p) is the index of prime p */
        indices[num_idx++] = (idx_t)pi;
    }

    if (num_idx == 0) return (seq_t)1; /* should not occur unless n=1 */

    /* Count maximal independent sets for each component of consecutive indices */
    seq_t result = (seq_t)1;
    size_t run_len = 1;
    for (size_t i = 1; i < num_idx; ++i) {
        if (indices[i] == indices[i-1] + 1)
            ++run_len;
        else {
            result *= mis_count_of_len(run_len);
            run_len = 1;
        }
    }
    result *= mis_count_of_len(run_len);

    return result;
}
