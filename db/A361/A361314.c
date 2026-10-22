/*
 * A361314  a(1) = 1, a(2) = 2; for n > 2, a(n) is the smallest positive
 *          number which has not appeared that shares a factor with a(n-2) +
 *          a(n-1) while the sum a(n) + a(n-1) is distinct from all previous
 *          sums a(i) + a(i-1), i=2..n-1.
 * https://oeis.org/A361314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence with search a(1)=1, a(2)=2 For n>2, a(n) is the
 *            smallest positive integer not yet used that shares a non‑trivial
 *            factor with a(n-2)+a(n-1) and whose sum with a(n-1) has never
 *            appeared before
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"   /* provides: seq_t gcd(seq_t a, seq_t b); */

/* Static storage for the sequence and helper flags. */
static seq_t *seq = NULL;          /* array of computed terms   */
static size_t seq_len = 0;         /* number of terms stored   */
static size_t seq_cap = 0;         /* capacity of seq array    */

static unsigned char *used = NULL; /* used[n]==1 ⇔ n already appears */
static size_t used_cap = 0;

static unsigned char *sum_used = NULL; /* sum_used[s]==1 ⇔ sum s already seen */
static size_t sum_cap = 0;

/* Ensure that seq has capacity for indices up to min_index (inclusive). */
static void expand_seq(size_t min_index) {
    if (min_index >= seq_cap) {
        size_t new_cap = seq_cap ? seq_cap : 8;
        while (new_cap <= min_index) new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_ptr) abort();
        seq = new_ptr;
        seq_cap = new_cap;
    }
}

/* Ensure that used has capacity for indices up to min_index (inclusive). */
static void expand_used(size_t min_index) {
    if (min_index >= used_cap) {
        size_t old_cap = used_cap;
        size_t new_cap = old_cap ? old_cap : 16;
        while (new_cap <= min_index) new_cap <<= 1;
        unsigned char *new_ptr = (unsigned char *)realloc(used, new_cap);
        if (!new_ptr) abort();
        memset(new_ptr + old_cap, 0, new_cap - old_cap);
        used = new_ptr;
        used_cap = new_cap;
    }
}

/* Ensure that sum_used has capacity for indices up to min_index (inclusive). */
static void expand_sum_used(size_t min_index) {
    if (min_index >= sum_cap) {
        size_t old_cap = sum_cap;
        size_t new_cap = old_cap ? old_cap : 32;
        while (new_cap <= min_index) new_cap <<= 1;
        unsigned char *new_ptr = (unsigned char *)realloc(sum_used, new_cap);
        if (!new_ptr) abort();
        memset(new_ptr + old_cap, 0, new_cap - old_cap);
        sum_used = new_ptr;
        sum_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset 1, undefined for 0 */

    /* Initialise base cases once. */
    if (seq_len == 0) {
        expand_seq(1);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq_len = 2;

        expand_used(2);
        used[1] = 1;
        used[2] = 1;

        /* a(2)+a(1)=3 is now used. */
        expand_sum_used(3);
        sum_used[3] = 1;
    }

    /* Build the sequence up to the requested index. */
    while (seq_len < (size_t)n) {
        seq_t prevPrev = seq[seq_len - 2];
        seq_t prev     = seq[seq_len - 1];
        seq_t required_gcd = prevPrev + prev;   /* a(n-2) + a(n-1) */

        /* Find the smallest unused candidate with a common factor with
           required_gcd and a new sum when added to prev. */
        seq_t candidate = (seq_t)1;
        for (;;) {
            size_t cand_idx = (size_t)candidate;
            expand_used(cand_idx);
            if (used[cand_idx]) { ++candidate; continue; }

            if (gcd(candidate, required_gcd) == (seq_t)1) { ++candidate; continue; }

            seq_t new_sum = candidate + prev;
            size_t sum_idx = (size_t)new_sum;
            expand_sum_used(sum_idx);
            if (sum_used[sum_idx]) { ++candidate; continue; }

            /* Suitable candidate found. */
            break;
        }

        /* Store the new term. */
        expand_seq(seq_len);      /* guarantees space for seq[seq_len] */
        seq[seq_len] = candidate;
        seq_len++;

        /* Mark the value and its sum with the previous term as used. */
        used[(size_t)candidate]                 = 1;
        seq_t prev_val = seq[seq_len - 2];      /* previous term after insert */
        sum_used[(size_t)(candidate + prev_val)] = 1;
    }

    return seq[(size_t)n - 1];
}
