/*
 * A297838  Solution (a(n)) of the system of 3 complementary equations in
 *          Comments.
 * https://oeis.org/A297838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   ensure_used
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297838
   Recurrence:  a(0)=1, b(0)=2.
   For n>0:
     a(n) = least unused positive integer,
     b(n) = least unused integer ≥ a(n)+n+1,
     c(n) = a(n)+b(n).
   This implementation builds the sequences on demand with a bitset
   of used values; it memoises computed a(n) values so each index is
   processed only once, regardless of call order.  The implementation
   works for any seq_t/idx_t combination that fits the compiler
   constraints specified in the problem. */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ensure_used.h"

/* Ensure that an array of seq_t can address index 'need' (inclusive).  */
static void ensure_arr(idx_t need, seq_t **arr, idx_t *cap)
{
    while (*cap <= need) {
        idx_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Return a(n) for A297838.  */
seq_t a(idx_t n)
{
    static unsigned char *used = NULL;
    static idx_t used_cap = 0;
    static seq_t *a_arr = NULL;
    static idx_t a_cap = 0;
    static idx_t max_n = 0;
    static idx_t candidate = 1;  /* next unused integer to examine */

    /* First call: initialise data structures.  */
    if (!used) {
        used = (unsigned char *)calloc(4, sizeof(unsigned char));
        if (!used) exit(1);
        used_cap = 4;
        /* mark 1,2,3 as used for the base case.  */
        used[1] = used[2] = used[3] = 1;
        candidate = 4;

        a_arr = (seq_t *)calloc(1, sizeof(seq_t));
        if (!a_arr) exit(1);
        a_cap = 1;
        a_arr[0] = (seq_t)1;
        max_n = 0;
    }

    if (n > max_n) {
        /* Grow the a array if necessary.  */
        ensure_arr(n, &a_arr, &a_cap);

        /* Compute new terms up to n.  */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* Find a(i) = least unused integer.  */
            ensure_used(candidate, &used, &used_cap);
            while (used[candidate]) {
                ++candidate;
                ensure_used(candidate, &used, &used_cap);
            }
            seq_t ai = (seq_t)candidate;
            idx_t ai_idx = candidate;

            /* Find b(i) = least unused ≥ a(i)+i+1.  */
            idx_t bi_start = ai_idx + i + 1;
            ensure_used(bi_start, &used, &used_cap);
            idx_t bi_idx = bi_start;
            while (used[bi_idx]) {
                ++bi_idx;
                ensure_used(bi_idx, &used, &used_cap);
            }
            seq_t bi = (seq_t)bi_idx;

            /* Compute c(i) = a(i) + b(i).  */
            idx_t ci_idx = ai_idx + bi_idx;
            ensure_used(ci_idx, &used, &used_cap);
            seq_t ci = (seq_t)ci_idx;

            /* Mark a, b, c as used.  */
            used[ai_idx] = 1;
            used[bi_idx] = 1;
            used[ci_idx] = 1;

            /* Store a(i).  */
            a_arr[i] = ai;

            /* Prepare candidate for the next iteration.  */
            candidate = ai_idx + 1;
            ensure_used(candidate, &used, &used_cap);
        }
        max_n = n;
    }

    return a_arr[n];
}
