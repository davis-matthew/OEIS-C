/*
 * A260577  Numbers n for which d(n+d(n)) < d(n), where d(n) is the number of
 *          divisors of n.
 * https://oeis.org/A260577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for numbers n with d(n+d(n)) < d(n) */

/* Note: Uses divisor_count_u64 from ../utils/divisor_count_u64.h */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/divisor_count_u64.h"

/* static storage for sequence terms */
static seq_t *terms = NULL;     /* array of found terms */
static size_t terms_len = 0;    /* number of terms stored */
static size_t terms_cap = 0;    /* current allocated capacity */
static uint64_t last_checked = 0;  /* last integer inspected */
static bool initialized = false;   /* initialization flag */

/* enlarge the terms array when necessary */
static void grow_terms(void) {
    terms_cap = terms_cap ? terms_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
}

/* ensure that the static structures are allocated once */
static void ensure(void) {
    if (initialized) return;

    terms_cap = 16;
    terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    if (!terms) abort();

    terms_len = 0;
    last_checked = 0;          /* start scanning from 1 */
    initialized = true;
}

/* fill the terms array until it contains at least target_len elements */
static void compute_more_terms(size_t target_len) {
    while (terms_len < target_len) {
        /* search forward for the next qualifying integer */
        for (;;) {
            ++last_checked;
            uint64_t d1 = divisor_count_u64(last_checked);
            uint64_t m  = last_checked + d1;
            uint64_t d2 = divisor_count_u64(m);
            if (d2 < d1) {
                /* store the found term */
                if (terms_len == terms_cap) grow_terms();
                terms[terms_len++] = (seq_t)last_checked;
                break; /* move to next desired term */
            }
        }
    }
}

/* main sequence routine, idx_t is unsigned (uint64_t) */
seq_t a(idx_t n) {
    if (n == 0)          /* sequence is offset 1; 0 is unused */
        return (seq_t)0;
    ensure();
    compute_more_terms((size_t)n);
    return terms[(size_t)n - 1];
}
