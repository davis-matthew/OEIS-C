/*
 * A375868  E.g.f. satisfies A(x) = exp( 2 * (exp(x*A(x)) - 1) ).
 * https://oeis.org/A375868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k=1..n} (2n+2)^(k-1) * S(n,k), where S(n,k) are
 *            Stirling numbers of the second kind
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* static state */
    static int init = 0;
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cached_max = 0;

    static seq_t *prev = NULL;
    static idx_t prev_cap = 0;
    static seq_t *cur = NULL;
    static idx_t cur_cap = 0;

    if (!init) {
        init = 1;
        /* initialize cache for a(0) */
        cache_cap = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        cache[0] = 1;
        cached_max = 0;
        /* initialize previous row (S(0,0) = 1) */
        prev_cap = 1;
        prev = (seq_t*)malloc(sizeof(seq_t));
        prev[0] = 1;
        cur = NULL;
        cur_cap = 0;
    }

    if (n <= cached_max)
        return cache[n];

    /* compute a(k) for k = cached_max+1 .. n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* enlarge cur array to size i+1 if needed */
        if ((i + 1) > cur_cap) {
            cur = (seq_t*)realloc(cur, (i + 1) * sizeof(seq_t));
            cur_cap = i + 1;
        }

        cur[0] = 0;
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = 0;
            if (k < prev_cap)   /* k * prev[k] */
                term = (seq_t)k * prev[k];
            /* + prev[k-1] */
            term += prev[k - 1];
            cur[k] = term;
        }

        /* compute a(i) = 2 * sum_{k=1..i} (2i+2)^(k-1) * cur[k] */
        seq_t sum = 0;
        seq_t power = 1;
        seq_t base = (seq_t)(2 * i + 2);
        for (idx_t k = 1; k <= i; ++k) {
            sum += power * cur[k];
            power *= base;  /* next power (2i+2)^(k) */
        }
        seq_t ai = 2 * sum;

        /* enlarge cache if needed */
        if ((i + 1) > cache_cap) {
            cache = (seq_t*)realloc(cache, (i + 1) * sizeof(seq_t));
            cache_cap = i + 1;
        }
        cache[i] = ai;

        /* swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
        idx_t tmp_cap = prev_cap;
        prev_cap = cur_cap;
        cur_cap = tmp_cap;
    }

    cached_max = n;
    return cache[n];
}
