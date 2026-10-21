/*
 * A296257  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2)^2, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(n)=a(n-1)+a(n-2)+b(n-2)^2 (n>=2); where b(0)=3
 *            and the sequences a() and b() are complementary increasing
 *            sequences
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 178,
 *            big any n
 * Helpers:   seq_in_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_in_array.h"

/* Static caches for a and b */
static seq_t *a_cache = NULL;
static size_t a_cap   = 0;
static size_t a_len   = 0;

static seq_t *b_cache = NULL;
static size_t b_cap   = 0;
static size_t b_len   = 0;

/* Next candidate number for b generation (as seq_t) */
static seq_t next_candidate = 1;

static void ensure_a(idx_t n);
static void ensure_b(idx_t n);

/* Main function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    ensure_a(n);
    return a_cache[n];
}

/* Ensure a_cache contains a[0..n] */
static void ensure_a(idx_t n) {
    if (a_len > (size_t)n) return;  /* already computed */

    /* initialize a_cache if first call */
    if (a_cache == NULL) {
        a_cap = 8;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_len = 2;
    }

    /* compute missing terms */
    while (a_len <= (size_t)n) {
        /* ensure required b value is available */
        if (a_len >= 2)
            ensure_b(a_len - 2);

        /* compute a[a_len] = a[a_len-1] + a[a_len-2] + b[a_len-2]^2 */
        seq_t term1 = a_cache[a_len - 1];
        seq_t term2 = a_cache[a_len - 2];
        seq_t bval  = b_cache[a_len - 2];
        seq_t b2    = bval * bval;
        seq_t new_a = term1 + term2 + b2;

        /* grow a_cache if necessary */
        if (a_len == a_cap) {
            a_cap *= 2;
            a_cache = (seq_t *)realloc(a_cache, a_cap * sizeof(seq_t));
        }
        a_cache[a_len] = new_a;
        a_len++;
    }
}

/* Ensure b_cache contains b[0..n] */
static void ensure_b(idx_t n) {
    if (b_len > (size_t)n) return;  /* already computed */

    /* initialize b_cache if first call */
    if (b_cache == NULL) {
        b_cap = 8;
        b_cache = (seq_t *)malloc(b_cap * sizeof(seq_t));
        b_len = 0;
        next_candidate = 1;  /* start scanning from 1 */
    }

    while (b_len <= (size_t)n) {
        /* find next natural number not in a_cache (complement) */
        do {
            /* increment candidate */
            next_candidate = next_candidate + (seq_t)1;
        } while (seq_in_array(a_cache, a_len, next_candidate));

        /* add to b_cache */
        if (b_len == b_cap) {
            b_cap *= 2;
            b_cache = (seq_t *)realloc(b_cache, b_cap * sizeof(seq_t));
        }
        b_cache[b_len] = next_candidate;
        b_len++;
    }
}
