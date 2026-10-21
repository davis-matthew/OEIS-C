/*
 * A296256  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1)^2, where a(0) = 3, a(1) = 4, b(0) = 1, b(1) = 2, and (a(n))
 *          and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A296256
   a(0) = 3, a(1) = 4
   a(n) = a(n-1) + a(n-2) + b(n-1)^2
   b(0) = 1, b(1) = 2
   (a(n)) and (b(n)) are increasing complementary sequences.  */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void grow_seq_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

/* ---------------------------------------------------------------------- */
/* Main sequence function                                                 */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_vals = NULL, *b_vals = NULL;
    static size_t a_cap = 0, b_cap = 0;
    static size_t a_len = 0, b_len = 0;

    static seq_t last_b = 0;
    static seq_t next_b_candidate = 0;
    static idx_t ptr_a = 0;          /* index into a_vals used for b‑search */
    static int initialized = 0;

    /* first run initialises the sequences */
    if (!initialized) {
        grow_seq_array(&a_vals, &a_cap, 2);
        grow_seq_array(&b_vals, &b_cap, 2);

        a_vals[0] = (seq_t)3;
        a_vals[1] = (seq_t)4;
        a_len = 2;

        b_vals[0] = (seq_t)1;
        b_vals[1] = (seq_t)2;
        b_len = 2;

        last_b = (seq_t)2;
        next_b_candidate = (seq_t)3;
        ptr_a = 0;

        initialized = 1;
    }

    /* extend the tables up to index n */
    while (a_len <= n) {
        /* calculate a_next = a(n-1)+a(n-2)+b(n-1)^2 */
        seq_t a_prev1 = a_vals[a_len - 1];
        seq_t a_prev2 = a_vals[a_len - 2];
        seq_t b_prev   = b_vals[b_len - 1];

        seq_t a_next = a_prev1 + a_prev2 + b_prev * b_prev;

        /* store it */
        grow_seq_array(&a_vals, &a_cap, a_len + 1);
        a_vals[a_len] = a_next;
        a_len++;

        /* find next b as the smallest integer > last_b not in a_vals */
        seq_t candidate = next_b_candidate;

        while (1) {
            /* advance ptr_a past a values smaller than candidate */
            while (ptr_a < a_len && a_vals[ptr_a] < candidate)
                ptr_a++;

            if (ptr_a < a_len && a_vals[ptr_a] == candidate) {
                /* candidate is used by a; skip it */
                candidate++;
                continue;
            }
            /* candidate is free */
            break;
        }

        /* store b_next */
        grow_seq_array(&b_vals, &b_cap, b_len + 1);
        b_vals[b_len] = candidate;
        b_len++;

        /* update for next iteration */
        last_b = candidate;
        next_b_candidate = candidate + 1;
    }

    return a_vals[n];
}
