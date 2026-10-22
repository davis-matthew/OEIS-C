/*
 * A341989  Number of ways to write n as an ordered sum of 10 primes (counting
 *          1 as a prime).
 * https://oeis.org/A341989
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (x + Σ_{prime p} x^p)^10 * (a(10)
 *            is the first term, indices below 10 are not defined)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   build_primes, zero_seq_vec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* External helpers */
#include "../utils/build_primes.h"
#include "../utils/zero_seq_vec.h"

/* ------------------------------------------------------------------ */
/*  State: cache of computed values and primes                         */
/* ------------------------------------------------------------------ */
static seq_t *dp_cache      = NULL;   /* a(k) for k = 0 .. cap-1   */
static size_t dp_cap        = 0;       /* current allocated size    */
static uint64_t *prime_list = NULL;    /* primes up to current limit */
static size_t prime_cnt     = 0;       /* number of primes in list  */

/* ------------------------------------------------------------------ */
/*  Utility: zero an array of seq_t                                    */

/* ------------------------------------------------------------------ */
/*  Main sequence function                                             */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Offset: values for n < 10 are undefined for the problem */
    if (n < 10) return 0;

    /* Requested sum */
    const size_t target = (size_t)n;

    /* If we already have the value computed, return it */
    if (dp_cache && target < dp_cap) {
        return dp_cache[target];
    }

    /* Extend the cache to cover up to target */
    const size_t new_cap = target + 1;   /* array size needed */

    /* -------------------- primes --------------------------------- */
    /* Free old prime list if any */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }

    /* Build primes up to numeric limit n (i.e., new_cap-1) */
    build_primes((idx_t)(new_cap - 1), (idx_t **)&prime_list, &prime_cnt);

    /* -------------------- DP arrays -------------------------------- */
    seq_t *prev = (seq_t *)malloc(new_cap * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc(new_cap * sizeof(seq_t));

    /* Initial condition: k=0 (no primes used) */
    zero_seq_vec(prev, new_cap);
    prev[0] = 1;   /* one way to sum to 0 with 0 primes */

    /* Perform 10 steps of convolution */
    for (int step = 1; step <= 10; ++step) {
        zero_seq_vec(curr, new_cap);

        /* Handle the "prime" 1 explicitly */
        for (size_t s = 0; s < new_cap - 1; ++s) {
            if (prev[s] != 0) {
                curr[s + 1] += prev[s];
            }
        }

        /* Handle real primes (start from 2) */
        for (size_t pi = 0; pi < prime_cnt; ++pi) {
            size_t p = (size_t)prime_list[pi];
            if (p > target) break;
            for (size_t s = 0; s <= target - p; ++s) {
                if (prev[s] != 0) {
                    curr[s + p] += prev[s];
                }
            }
        }

        /* Swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* 'prev' now contains a(k) for k = 0 .. target */
    /* Allocate/reallocate cache and copy results */
    if (dp_cache) {
        free(dp_cache);
    }
    dp_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    for (size_t i = 0; i < new_cap; ++i) {
        dp_cache[i] = prev[i];
    }

    /* Clean up temporary arrays */
    free(prev);
    free(curr);

    /* Update cache size */
    dp_cap = new_cap;

    /* Return the requested value */
    return dp_cache[target];
}
