/*
 * A381014  If n = Product (p_j^k_j) then a(n) = Sum partition(p_j^k_j).
 * https://oeis.org/A381014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum partition(p_j^k_j) for the prime factorisation * n =
 *            ∏ p_j^k_j. Partition(p) is the number of integer partitions of *
 *            p (order of parts irrelevant). For n = 1 the sum is empty, so
 *            a(1)=0
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* factor_u64 is provided in the OEIS utils repository. */
#include "../utils/factor_u64.h"

/* Cached partition numbers.  part_cache[i] holds p(i) for i ≤ part_max. */
static seq_t *part_cache = NULL;
static idx_t   part_max  = 0;

/* Recompute all partitions up to 'limit' (inclusive) by the
 * standard coin‑change dynamic programme.  This uses only additions,
 * so it works for unsigned seq_t types as well as arbitrary‑precision. */
static void recompute_partitions(idx_t limit)
{
    /* Allocate or enlarge the cache.  Use a cast in C++ as required. */
    size_t new_cap = (size_t)(limit + 1);
    seq_t *tmp = (seq_t*)realloc(part_cache, new_cap * sizeof(seq_t));
    /* In the unlikely event of malloc failure exit immediately. */
    if (!tmp) { exit(EXIT_FAILURE); }

    part_cache = tmp;

    /* Initialise the table.  Every entry becomes 0; we then set part[0]=1. */
    for (idx_t i = 0; i <= limit; ++i) part_cache[i] = 0;
    part_cache[0] = 1;

    /* Classic DP: for each coin value i add it to all larger totals. */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; ++j) {
            part_cache[j] = part_cache[j] + part_cache[j - i];
        }
    }
    part_max = limit;
}

/* Ensure that partition numbers up to 'n' exist in the cache. */
static void ensure_partitions_upto(idx_t n)
{
    if (n <= part_max) return;
    recompute_partitions(n);
}

/* --------------------------------------------------------------------- */
/* Main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n = 1 has no prime factors → sum = 0. */
    if (n == 0) return 0;   /* safety guard – OEIS offset starts at 1 */

    /* Allocate temporary arrays for factorisation.  64 primes is sufficient
     * for all n ≤ 2^64, because 2^64 has at most 64 prime factors (all 2). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;

    /* For each prime power, add its partition number. */
    for (size_t i = 0; i < cnt; ++i) {
        /* Compute p^k as idx_t (fits in uint64_t because n itself fits). */
        idx_t power = 1;
        for (size_t e = 0; e < exps[i]; ++e) {
            power *= primes[i];
        }

        ensure_partitions_upto(power);

        sum = sum + part_cache[power];
    }
    return sum;
}
