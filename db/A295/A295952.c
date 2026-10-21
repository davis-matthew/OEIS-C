/*
 * A295952  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n), where a(0) = 1, a(1) = 5, b(0) = 2, b(1) = 3, b(2) = 4, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑2) + b(n) Initial values: a(0)=1, a(1)=5,
 *            b(0)=2, b(1)=3, b(2)=4 (a(n)) and (b(n)) are increasing
 *            complementary sequences
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   seq_in
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_in.h"

/* ensure that array has space for needed items */
static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap == 0) ? 1 : *cap;
    while (new_cap < needed) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    static seq_t *a_vals = NULL;
    static size_t a_len = 0, a_cap = 0;
    static seq_t *b_vals = NULL;
    static size_t b_len = 0, b_cap = 0;
    static seq_t next_b_candidate = 0;   /* next integer to try for b */

    /* initialise if first call */
    if (a_len == 0) {
        a_cap = 2;
        a_vals = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_vals[0] = (seq_t)1;
        a_vals[1] = (seq_t)5;
        a_len = 2;
        /* initialise b */
        b_cap = 3;
        b_vals = (seq_t *)malloc(b_cap * sizeof(seq_t));
        b_vals[0] = (seq_t)2;
        b_vals[1] = (seq_t)3;
        b_vals[2] = (seq_t)4;
        b_len = 3;
        /* next candidate after the known b values */
        next_b_candidate = (seq_t)5;
    }

    if ((idx_t)n < (idx_t)a_len) return a_vals[n];

    /* generate terms up to n */
    while ((idx_t)a_len <= n) {
        /* Ensure b[a_len] exists */
        if (b_len <= a_len) {
            seq_t cur = next_b_candidate;
            /* skip if cur already in a or b */
            while (seq_in(a_vals, a_len, cur) || seq_in(b_vals, b_len, cur))
                ++cur;
            ensure_cap(&b_vals, &b_cap, b_len + 1);
            b_vals[b_len] = cur;
            ++b_len;
            next_b_candidate = cur + 1;
        }

        /* compute next a */
        seq_t new_a = a_vals[a_len - 1] + a_vals[a_len - 2] + b_vals[a_len];
        ensure_cap(&a_vals, &a_cap, a_len + 1);
        a_vals[a_len] = new_a;
        ++a_len;
    }

    return a_vals[n];
}
