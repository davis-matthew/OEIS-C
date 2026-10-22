/*
 * A367116  a(0) = 1; for n >= 1, a(n) is the largest number m =
 *          Product_{j=1..k} (b(j)-1) where {b(1), ..., b(k)} is a vector of
 *          positive integers such that Sum_{i=1..k} b(i)^2 = n.
 * https://oeis.org/A367116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 143, u32 n <= 287, u64 n <= 575,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A367116: a(0)=1; for n≥1, a(n)=max_{i^2≤n,(i≥2)} (i-1)*a(n-i^2).
   Computed by dynamic programming. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   // size of allocated array
    static idx_t last = (idx_t)-1; // highest index computed

    /* initialise cache for n=0 */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
        last = 0;
    }

    /* grow cache if needed */
    if ((idx_t)cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (tmp == NULL) {
            /* allocation failure – abort (not expected in test harness) */
            abort();
        }
        cache = tmp;
        cache_len = new_len;
    }

    /* compute new values up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        seq_t best = (seq_t)0;
        /* i starts from 2 because i=1 would give factor 0 */
        for (idx_t i = 2; i * i <= k; ++i) {
            idx_t rem = k - i * i;
            seq_t prod = ((seq_t)(i - 1)) * cache[rem];
            if (prod > best) best = prod;
        }
        cache[k] = best;
    }
    last = n;
    return cache[n];
}
