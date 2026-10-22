/*
 * A317508  Number of ways to split the integer partition with Heinz number n
 *          into consecutive subsequences with weakly decreasing sums.
 * https://oeis.org/A317508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to split the partition obtained from the
 *            prime‑index multiset of n (sorted in decreasing order) into
 *            consecutive subsequences whose sums are weakly decreasing
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, split_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We rely on the utility functions that are available in the build,
   notably factor_u64, build_primes_upto, and the integer type seq_t. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"            /* factor_u64 */
#include "../utils/build_primes_upto.h"     /* build_primes_upto */
#include "../utils/split_rec.h"

static uint64_t *global_prime_list = NULL;
static size_t   global_prime_cnt   = 0;

/* Find the 1‑based index of prime p in global_prime_list. */
/* p is guaranteed to be prime. */
static inline size_t prime_index(uint64_t p)
{
    size_t lo = 0;
    size_t hi = global_prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (global_prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < global_prime_cnt && global_prime_list[lo] == p)
        return lo + 1;        /* 1‑based index */
    return 0;                 /* should never happen for a valid n */
}

/* Recursive function that counts the weakly decreasing splits. */

seq_t a(idx_t n)
{
    /* Initialise prime list on first call.  We need primes up to at
       least the largest prime factor of numbers up to the maximum n
       that may be queried.  Using 20000 comfortably covers all n
       up to about 10 000. */
    if (global_prime_list == NULL) {
        build_primes_upto(20000, &global_prime_list, &global_prime_cnt);
    }

    /* Special case: n == 1 has no prime factors; its only partition
       is empty, giving one trivial split. */
    if (n == 1)
        return (seq_t)1;

    /* Factor n into primes.  Use a small buffer; n <= 10 000 so at
       most 13 prime factors (2^13 = 8192 < 10 000 < 2^14). */
    uint64_t fac_primes[64];
    uint64_t fac_exps[64];
    size_t   fac_cnt = 0;
    factor_u64(n, fac_primes, fac_exps, &fac_cnt);

    /* Build the multiset of prime indices according to the factor
       multiplicities. */
    unsigned parts[64];
    size_t part_cnt = 0;
    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p   = fac_primes[i];
        size_t   exp = (size_t)fac_exps[i];
        size_t   idx = prime_index(p);   /* 1‑based index of prime p */
        for (size_t k = 0; k < exp; ++k)
            parts[part_cnt++] = (unsigned)idx;
    }

    /* Sort parts in decreasing order – required by the definition. */
    for (size_t i = 1; i < part_cnt; ++i) {
        unsigned key = parts[i];
        size_t j = i;
        while (j > 0 && parts[j - 1] < key) {
            parts[j] = parts[j - 1];
            --j;
        }
        parts[j] = key;
    }

    /* Count all weakly decreasing splits of the sorted multiset. */
    return split_rec(parts, part_cnt, 0, (seq_t)0);
}
