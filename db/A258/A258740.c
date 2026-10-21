/*
 * A258740  Lucas numbers whose sum of decimal digits is greater than its
 *          index.
 * https://oeis.org/A258740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A258740.
   The term a(n) is the nth Lucas number L(k) (k ≥ 2) such that
   digit_sum(L(k)) > k.  If the requested n exceeds the length of
   the sequence the function returns 0.  The implementation keeps
   a static cache of the terms that have been discovered so far. */
#include <stdbool.h>
#include <stdlib.h>

/* seq_t, idx_t and helper functions are supplied by the build system. */
#include "../utils/digit_sum.h"

/* Return the nth term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    /* Static state preserved between calls. */
    static bool initialized = false;
    static idx_t cur_k = 1;                 /* index of L_prev1 (initially L(1)) */
    static seq_t L_prev2 = 2;               /* L(0)  */
    static seq_t L_prev1 = 1;               /* L(1)  */
    static seq_t *terms   = NULL;           /* cached qualifying Lucas numbers */
    static size_t terms_count = 0;
    static size_t terms_cap   = 0;

    /* Allocate cache on first use. */
    if (!initialized) {
        initialized = true;
    }

    /* If we already have the requested term, return it. */
    if (n > 0 && terms_count >= (size_t)n) {
        return terms[(size_t)n - 1];
    }

    /* Generate Lucas numbers until we have enough terms or reach a stop
       condition (e.g. overflow or a hard bound). */
    while (terms_count < (size_t)n) {
        /* Compute next Lucas number: L(k) = L(k-1) + L(k-2). */
        seq_t L_next = L_prev1 + L_prev2;

        /* Detect unsigned wrap‑around: if the sum wrapped, stop. */
        if (L_next < L_prev1) {
            break;
        }

        /* Increment the index to match the newly computed value. */
        ++cur_k; /* now cur_k is the index of L_next */

        /* Check the defining inequality. */
        if (digit_sum(L_next) > cur_k) {
            /* Grow the cache if necessary. */
            if (terms_count >= terms_cap) {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – give up. */
                    return 0;
                }
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_count++] = L_next;
        }

        /* Advance the Lucas recurrence. */
        L_prev2 = L_prev1;
        L_prev1 = L_next;

        /* Safe hard limit to avoid infinite loops if the sequence were
           unbounded; the known maximum index is 6903, so 10 million is
           safely above that. */
        if (cur_k > 10000000U) {
            break;
        }
    }

    /* Return the requested term if we managed to find it. */
    if (n > 0 && terms_count >= (size_t)n) {
        return terms[(size_t)n - 1];
    }

    /* Not enough terms have been found – the sequence is exhausted. */
    return 0;
}
