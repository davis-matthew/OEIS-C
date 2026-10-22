/*
 * A390609  Irregular triangle read by rows: T(n,k) is the sum of noncomposite
 *          numbers in the k-th 2-dense sublist of divisors of n, with n >= 1,
 *          k >= 1.
 * https://oeis.org/A390609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 618, u16 n <= 9573, u32 n <= 9573, u64 n <= 9573,
 *            u128 n <= 9573, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements sequence A390609: 
   flatten the triangle T(n,k) that gives the sum of non‑composite
   divisors in each maximal 2‑dense sublist of the divisors of n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/*  True for non‑composite numbers (1 or prime). */
static inline bool is_noncomposite(idx_t d) {
    if (d == 1) return true;
    return is_prime(d);
}

/*  Pattern: iterate over rows n = 1,2,…, and emit all segment sums
    in order until at least n terms have been produced. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* safety – offset starts at 1 */

    /* static cache of already generated terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_row = 1;              /* next n to process */

    /* grow the cache as needed */
    while (cache_len < n) {
        idx_t r = next_row++;               /* current row number */

        /* walk through all divisors of r in ascending order */
        seq_t segsum = 0;                   /* sum for current 2‑dense segment */
        idx_t prev = 0;                     /* previous divisor in sequence */

        for (idx_t d = 1; d <= r; ++d) {
            if (r % d != 0) continue;      /* skip non‑divisors */

            if (prev == 0) {                /* first divisor in the row */
                segsum = is_noncomposite(d) ? (seq_t)d : 0;
                prev   = d;
            } else if (d <= prev * ((idx_t)2)) {
                /* still within the current 2‑dense sublist */
                if (is_noncomposite(d)) segsum += (seq_t)d;
                prev = d;
            } else {
                /* end of current sublist – store its sum */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!cache) exit(1);
                }
                cache[cache_len++] = segsum;

                /* start a new sublist with this divisor */
                segsum = is_noncomposite(d) ? (seq_t)d : 0;
                prev   = d;
            }
        }

        /* store the sum of the last sublist in the row */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!cache) exit(1);
        }
        cache[cache_len++] = segsum;
    }

    /* return the nth term (1‑based). */
    return cache[(size_t)(n - 1)];
}
