/*
 * A345164  Number of alternating permutations of the multiset of prime
 *          factors of n.
 * https://oeis.org/A345164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of alternating permutations of the multiset of prime factors of n.
   Implementation uses backtracking over the multiset of prime factors.
   Complexity is O(k! / Π(counts!)) where k is the total number of prime factors,
   which is at most about 13 for n ≤ 10000. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Recursive helper that counts alternating permutations.
   depth      : number of elements already placed.
   last_val   : value of the last placed element (irrelevant for depth=0).
   last_dir   : 0 for no previous direction, 1 for up, -1 for down. */
static seq_t dfs(size_t depth, uint64_t last_val, int last_dir,
                 const uint64_t *primes, int *counts,
                 size_t distinct, size_t k)
{
    if (depth == k)                /* all elements placed */
        return (seq_t)1;

    seq_t total = 0;
    for (size_t i = 0; i < distinct; ++i) {
        if (counts[i] == 0)
            continue;
        uint64_t val = primes[i];

        if (depth == 0) {          /* first element, no comparison */
            counts[i]--;
            total += dfs(depth + 1, val, 0, primes, counts, distinct, k);
            counts[i]++;
        } else {                   /* subsequent elements */
            if (val == last_val)
                continue;         /* strictness required */
            int dir_new = (val > last_val) ? 1 : -1;
            if (last_dir != 0 && dir_new == last_dir)
                continue;         /* must alternate directions */

            counts[i]--;
            total += dfs(depth + 1, val, dir_new, primes, counts, distinct, k);
            counts[i]++;
        }
    }
    return total;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1, but not expected */

    /* Factor n: primes_arr[k] and exps_arr[k] hold the factorization. */
    uint64_t primes_arr[64];
    uint64_t exps_arr[64];
    size_t factor_cnt = 0;
    factor_u64((uint64_t)n, primes_arr, exps_arr, &factor_cnt);

    /* Prepare count array and total length k of the multiset. */
    int counts[64];
    size_t k = 0;
    for (size_t i = 0; i < factor_cnt; ++i) {
        counts[i] = (int)exps_arr[i];
        k += exps_arr[i];
    }

    /* Count alternating permutations of the multiset. */
    return dfs(0, 0, 0, primes_arr, counts, factor_cnt, k);
}
