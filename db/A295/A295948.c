/*
 * A295948  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n), where a(0) = 3, a(1) = 4, b(0) = 1, b(1) = 2, b(2) = 5, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n), where b(n) is the least unused
 *            positive integer. Initial values: a(0)=3, a(1)=4; b(0)=1,
 *            b(1)=2, b(2)=5
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the memoized a-values */
static seq_t *a_vals   = NULL;
static size_t   a_len  = 0;
static size_t   a_cap  = 0;
/* The value of b(n-1) used for the next search */
static seq_t last_b   = 0;
/* Largest index of a which has been computed */
static idx_t  max_computed = 0;

/* Ensure the a array has room for at least 'need' elements */
static void ensure_a_capacity(size_t need) {
    if (need > a_cap) {
        size_t new_cap = a_cap > 0 ? a_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
        a_vals = tmp;
        a_cap  = new_cap;
    }
}

seq_t a(idx_t n) {
    /* Initialise the known starting terms on first call */
    if (max_computed == 0) {
        size_t init_cap = 16;
        a_vals = (seq_t *)malloc(init_cap * sizeof(seq_t));
        a_cap  = init_cap;

        a_vals[0] = (seq_t)3;
        a_vals[1] = (seq_t)4;
        a_len    = 2;
        max_computed = 1;
        last_b    = (seq_t)5;  /* b(2) */
    }

    if (n <= max_computed) {
        return a_vals[(size_t)n];
    }

    /* Compute terms up to the requested index */
    for (size_t idx = max_computed + 1; idx <= (size_t)n; ++idx) {
        seq_t b_val;

        if (idx == 2) {
            b_val = (seq_t)5;    /* known initial value */
            last_b = b_val;
        } else {
            /* Find the least unused positive integer for b(idx) */
            seq_t candidate = last_b + (seq_t)1;
            size_t a_idx = 0;
            while (1) {
                /* Skip a-values that are smaller than the candidate */
                while (a_idx < a_len && a_vals[a_idx] < candidate) {
                    ++a_idx;
                }
                /* If candidate equals an a-value, it is not unused */
                if (a_idx < a_len && a_vals[a_idx] == candidate) {
                    ++candidate;
                } else {
                    /* Candidate is unused, thus b(idx) */
                    b_val = candidate;
                    break;
                }
            }
            last_b = b_val;
        }

        /* a(idx) = a(idx-1) + a(idx-2) + b(idx) */
        seq_t a_val = a_vals[idx - 1] + a_vals[idx - 2] + b_val;

        /* Store the new term */
        ensure_a_capacity(idx + 1);
        a_vals[idx] = a_val;
        a_len = idx + 1;
    }

    max_computed = (idx_t)n;
    return a_vals[(size_t)n];
}
