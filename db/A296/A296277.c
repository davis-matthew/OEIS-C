/*
 * A296277  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1)*b(n), where a(0) = 3, a(1) = 4, b(0) = 1, b(1) = 2, b(2) =
 *          5, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1)*b(n); Initial values: a(0)=3,
 *            a(1)=4; b(0)=1, b(1)=2, b(2)=5
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Helpers:   seq_exists
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  External types provided by the build system.  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_exists.h"

/*  Static state.  */
static seq_t *a_arr     = NULL;   /* array of a-values   */
static seq_t *b_arr     = NULL;   /* array of b-values   */
static size_t a_cap = 0, b_cap = 0;
static size_t a_len = 0, b_len = 0;
static seq_t next_candidate = 0;  /* next integer to try for b */

/*  Simple dynamic array growth helper  */
static void grow_seq(seq_t **arr, size_t *cap, size_t min_cap) {
    if (*cap >= min_cap) return;
    size_t new_cap = *cap ? (*cap << 1) : 8;
    if (new_cap < min_cap) new_cap = min_cap;
    *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

/*  Return the next unused integer for the b-sequence.  */
static seq_t next_b_value(void) {
    seq_t v = next_candidate;
    while (seq_exists(a_arr, a_len, v)) {
        v++;                /* skip any a-value that equals v  */
    }
    next_candidate = v + 1; /* next call will start above v     */
    return v;
}

/*  One‑time initialization of the tables and the first few terms.  */
static void init(void) {
    if (a_arr) return;   /* already initialized */

    /*  Allocate a[] and store a(0) and a(1).  */
    grow_seq(&a_arr, &a_cap, 8);
    a_arr[0] = 3;
    a_arr[1] = 4;
    a_len = 2;

    /*  Allocate b[] and store b(0), b(1), b(2).  */
    grow_seq(&b_arr, &b_cap, 8);
    b_arr[0] = 1;
    b_arr[1] = 2;
    b_arr[2] = 5;
    b_len = 3;

    /*  next_candidate starts just after the last b we have.  */
    next_candidate = 6;
}

/*  Ensure that a[] contains all terms up to and including index n.  */
static void ensure_a_upto(idx_t n) {
    init();                      /* one‑time set up */
    while (a_len <= (size_t)n) {
        size_t i = a_len;                /* we will compute a[i] */

        /*  Make sure b[i] exists.  */
        if (b_len <= i) {
            seq_t bi = next_b_value();
            grow_seq(&b_arr, &b_cap, b_len + 1);
            b_arr[b_len++] = bi;
        }

        /*  Compute a[i] = a[i-1] + a[i-2] + b[i-1]*b[i]  */
        seq_t a_im1 = a_arr[i - 1];
        seq_t a_im2 = a_arr[i - 2];
        seq_t b_im1 = b_arr[i - 1];
        seq_t b_i   = b_arr[i];
        seq_t a_i   = a_im1 + a_im2 + b_im1 * b_i;

        grow_seq(&a_arr, &a_cap, a_len + 1);
        a_arr[a_len++] = a_i;
    }
}

/*  Public API required by the problem statement.  */
seq_t a(idx_t n) {
    ensure_a_upto(n);
    return a_arr[n];
}
