/*
 * A305712  Polydivisible nonnegative integers whose decimal digits span an
 *          initial interval of {0,...,9}.
 * https://oeis.org/A305712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   contiguous_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Algorithm: Breadth‑first generate all polydivisible numbers.
 * For each length, keep the numbers that are polydivisible
 * (prefix of length m divisible by m) and whose decimal digits
 * form the consecutive set {0,1,…,k}.  The filtered numbers are
 * stored in a cache; a(idx_t n) returns the nth term (a(0)=0).
 * This computes any term efficiently enough for n up to ~10000.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/contiguous_digits.h"

/* Ensure a dynamic array has at least 'need' elements. */
static void grow_seq(seq_t **ptr, size_t *cap, size_t need) {
    if (*cap < need) {
        size_t newcap = (*cap) ? (*cap) * 2 : 16;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(*ptr, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *ptr  = tmp;
        *cap  = newcap;
    }
}

/* Application entry point. */
seq_t a(idx_t n) {
    /* Static cache of requested terms */
    static seq_t *cache     = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_size = 0;

    /* Static storage of all polydivisible numbers of the current length */
    static seq_t *all_curr  = NULL;
    static size_t  all_curr_cap = 0;
    static size_t  all_curr_size = 0;
    static seq_t *all_next  = NULL;
    static size_t  all_next_cap = 0;
    static size_t  all_next_size = 0;
    static idx_t   curr_len   = 1;   /* length of numbers in all_curr */

    static bool initialized = false;

    if (!initialized) {
        /* initialise cache with first term 0 */
        grow_seq(&cache, &cache_cap, 16);
        cache[0] = 0;
        cache_size = 1;

        /* initialise all_curr with the 1‑digit numbers 1..9 */
        grow_seq(&all_curr, &all_curr_cap, 16);
        for (idx_t d = 1; d <= 9; ++d) all_curr[d-1] = (seq_t)d;
        all_curr_size = 9;
        curr_len = 1;
        initialized = true;
    }

    /* Grow until we have at least (n+1) cached terms */
    while (cache_size <= (size_t)n) {
        /* generate all polydivisible numbers of the next length */
        grow_seq(&all_next, &all_next_cap, all_curr_size * 10);
        all_next_size = 0;

        idx_t m = curr_len + 1;           /* new length */
        for (size_t i = 0; i < all_curr_size; ++i) {
            seq_t x   = all_curr[i];
            seq_t base = x * 10;
            for (int d = 0; d < 10; ++d) {
                seq_t y = base + d;
                if ((y % (seq_t)m) != 0) continue;  /* not polydivisible */
                /* store in all_next (for next iteration's parents) */
                grow_seq(&all_next, &all_next_cap, all_next_size + 1);
                all_next[all_next_size++] = y;
            }
        }

        /* filter for consecutive-digit property and add to cache */
        for (size_t j = 0; j < all_next_size; ++j) {
            if (contiguous_digits(all_next[j])) {
                grow_seq(&cache, &cache_cap, cache_size + 1);
                cache[cache_size++] = all_next[j];
            }
        }

        /* prepare for next iteration */
        all_curr_destroy:
        /* swap all_curr and all_next pointers and sizes */
        seq_t *tmp_ptr   = all_curr;
        size_t tmp_cap   = all_curr_cap;
        size_t tmp_size  = all_curr_size;

        all_curr = all_next;
        all_curr_cap = all_next_cap;
        all_curr_size = all_next_size;

        all_next = tmp_ptr;
        all_next_cap = tmp_cap;
        all_next_size = tmp_size;

        curr_len = m;
    }

    return cache[(size_t)n];
}
