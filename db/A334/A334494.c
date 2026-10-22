/*
 * A334494  Positions k of addition steps in Recamán's sequence A005132 where
 *          A005132(k-1)<k.
 * https://oeis.org/A334494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 5041, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A334494:
   a(n) = the n‑th position k where Recamán’s sequence
   performs an addition step and the previous term R(k-1) < k. */
#include <stdlib.h>

/* Global state for caching */
static seq_t *recam = NULL;      /* Recamán sequence values          */
static size_t recam_len = 0, recam_cap = 0;

static seq_t *seen_vals = NULL;  /* All values that have appeared   */
static size_t seen_len = 0, seen_cap = 0;

static idx_t *pos = NULL;        /* Positions k that meet the condition */
static size_t pos_len = 0, pos_cap = 0;

/* Ensure capacity for recam[] */
static void ensure_recam_cap(void) {
    if (recam_len >= recam_cap) {
        size_t new_cap = recam_cap ? recam_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(recam, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        recam = tmp;
        recam_cap = new_cap;
    }
}

/* Ensure capacity for seen_vals[] */
static void ensure_seen_cap(void) {
    if (seen_len >= seen_cap) {
        size_t new_cap = seen_cap ? seen_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(seen_vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seen_vals = tmp;
        seen_cap = new_cap;
    }
}

/* Ensure capacity for pos[] */
static void ensure_pos_cap(void) {
    if (pos_len >= pos_cap) {
        size_t new_cap = pos_cap ? pos_cap << 1 : 16;
        idx_t *tmp = (idx_t *)realloc(pos, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        pos = tmp;
        pos_cap = new_cap;
    }
}

/* Return the n‑th term of A334494 (offset 1). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* One‑time initialization */
    if (recam_cap == 0) {
        recam_cap = 1;
        recam = (seq_t *)calloc(1, sizeof(seq_t));
        if (!recam) abort();
        recam[0] = (seq_t)0;
        recam_len = 1;

        seen_cap = 1;
        seen_vals = (seq_t *)calloc(1, sizeof(seq_t));
        if (!seen_vals) abort();
        seen_vals[0] = (seq_t)0;
        seen_len = 1;

        pos_cap = 0;
        pos = NULL;
        pos_len = 0;
    }

    /* Generate until we have at least n qualifying positions */
    while (pos_len < n) {
        idx_t k = recam_len;                 /* Index of this step */
        seq_t prev = recam[recam_len - 1];
        seq_t next;
        int is_addition = 0;

        if (prev >= (seq_t)k) {
            seq_t cand = prev - (seq_t)k;
            /* Linear search to see if cand has been seen before */
            int found = 0;
            for (size_t i = 0; i < seen_len; ++i) {
                if (seen_vals[i] == cand) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                next = cand;          /* Subtraction is possible */
                is_addition = 0;
            } else {
                next = prev + (seq_t)k;  /* Must add */
                is_addition = 1;
            }
        } else {
            next = prev + (seq_t)k;
            is_addition = 1;
        }

        /* Append new Recamán value */
        ensure_recam_cap();
        recam[recam_len++] = next;

        /* Record the new value as seen */
        ensure_seen_cap();
        seen_vals[seen_len++] = next;

        /* Check the qualifying condition */
        if (is_addition && prev < (seq_t)k) {
            ensure_pos_cap();
            pos[pos_len++] = k;
        }
    }

    return (seq_t)pos[n - 1];
}
