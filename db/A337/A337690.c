/*
 * A337690  a(n) is the number of primitive nondeficient numbers (A006039)
 *          dividing n.
 * https://oeis.org/A337690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} 1 if d is a primitive nondeficient number. (Count
 *            of primitive nondeficient divisors of n.)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 8326 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Compute σ(n) for all n up to `limit` using a simple divisor-sum sieve. */
static void compute_sigma(uint64_t *sigma, idx_t limit) {
    /* sigma is assumed to be zero-initialized. */
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            sigma[j] += (uint64_t)i;
        }
    }
}

/* Recompute all arrays up to the new limit. */
static void recompute(idx_t new_limit,
                      bool **prim_arr,
                      seq_t **a_arr) {
    size_t sz = (size_t)(new_limit + 1);
    /* Allocate auxiliary arrays. */
    uint64_t *sigma = (uint64_t*)calloc(sz, sizeof(uint64_t));
    bool *nondef   = (bool*)calloc(sz, sizeof(bool));
    bool *prim     = (bool*)calloc(sz, sizeof(bool));
    *a_arr = mem_new(seq_t, sz);

    /* 1. Compute σ(n). */
    compute_sigma(sigma, new_limit);

    /* 2. Mark nondeficient numbers: σ(n) >= 2n. */
    for (idx_t n = 1; n <= new_limit; ++n) {
        nondef[n] = (sigma[n] >= 2ULL * (uint64_t)n);
    }

    /* 3. Initialise primitive flag to nondeficient. */
    for (idx_t n = 1; n <= new_limit; ++n) {
        prim[n] = nondef[n];
    }

    /* 4. Mark all multiples of any primitive nondeficient number
       as non-primitive. */
    for (idx_t n = 2; n <= new_limit; ++n) {          /* 1 is never primitive */
        if (prim[n]) {
            for (idx_t m = 2 * n; m <= new_limit; m += n) {
                prim[m] = false;
            }
        }
    }

    /* 5. Compute a(n) by adding 1 to all multiples of each primitive
       nondeficient number. */
    seq_t *a = *a_arr;
    for (idx_t d = 1; d <= new_limit; ++d) {
        if (prim[d]) {
            for (idx_t m = d; m <= new_limit; m += d) {
                a[m] += (seq_t)1;
            }
        }
    }

    /* 6. Free temporary arrays. */
    free(sigma);
    free(nondef);
    free(prim);
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* Static data to persist between calls. */
    static idx_t cur_limit = 0;
    static seq_t *a_arr = NULL;

    /* Recompute if we haven't covered this n yet. */
    if (n > cur_limit) {
        /* Clean up old data if any. */
        if (a_arr) {
            mem_free(a_arr);
            a_arr = NULL;
        }
        /* Compute new arrays up to n. */
        recompute(n, NULL, &a_arr);
        cur_limit = n;
    }

    return a_arr[n];
}
