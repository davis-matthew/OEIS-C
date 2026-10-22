/*
 * A320556  Number of set partitions of [n] such that for each block b the
 *          smallest integer interval containing b has at most six elements
 *          and for at least one block c the smallest integer interval
 *          containing c has exactly six elements.
 * https://oeis.org/A320556
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   calloc_seq, realloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is a(n) = A276722(n) – A276721(n), i.e. the number of
   set partitions of [n] whose largest block span is at most 6, minus
   the number whose largest block span is at most 5.  The implementation
   uses a DP on the last up to 6 positions; its state consists of a bit
   mask describing which of the last 5 (resp. 4) positions currently
   belong to the same block.  The DP runs in O(n · 32) time. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/calloc_seq.h"
#include "../utils/realloc_seq.h"

/* DP state sizes */
#define W6 6
#define W5 5
#define S6 (W6-1)          /* 5 */
#define S5 (W5-1)          /* 4 */
#define STATE6 (1U << S6)  /* 32 */
#define STATE5 (1U << S5)  /* 16 */

/* Masks to keep the lower bits after shifting left */
#define MASK6_KEEP (((1U << (S6-1)) - 1) << 1)  /* 0x1E */
#define MASK5_KEEP (((1U << (S5-1)) - 1) << 1)  /* 0x0E */

/* Global scratch buffers and result arrays */
static seq_t *dp6      = NULL;  /* DP for width 6 */
static seq_t *dp6_next = NULL;
static seq_t *dp5      = NULL;  /* DP for width 5 */
static seq_t *dp5_next = NULL;

static seq_t *arr6 = NULL;  /* answer for width 6 */
static seq_t *arr5 = NULL;  /* answer for width 5 */

static size_t arr6_alloc = 0;
static size_t arr5_alloc = 0;
static idx_t  computed_n = 0;   /* largest n that has been precomputed */

/* Ensure all tables have been computed up to n 'upto'. */
static void ensure_precomputed(idx_t upto)
{
    if (upto <= computed_n)
        return;

    /* Allocate result arrays if needed. */
    if (upto + 1 > arr6_alloc) {
        seq_t *new_arr = realloc_seq(arr6, upto + 1);
        if (!new_arr) exit(1);
        arr6 = new_arr;
        arr6_alloc = upto + 1;
    }
    if (upto + 1 > arr5_alloc) {
        seq_t *new_arr = realloc_seq(arr5, upto + 1);
        if (!new_arr) exit(1);
        arr5 = new_arr;
        arr5_alloc = upto + 1;
    }

    /* Allocate DP buffers if needed. */
    if (!dp6) {
        dp6      = calloc_seq(STATE6);
        dp6_next = calloc_seq(STATE6);
        dp5      = calloc_seq(STATE5);
        dp5_next = calloc_seq(STATE5);
        if (!dp6 || !dp6_next || !dp5 || !dp5_next) exit(1);

        /* initial condition: no positions processed, one empty partition */
        dp6[0] = dp5[0] = 1;
        arr6[0] = arr5[0] = 1;
        computed_n = 0;
    }

    /* Process successive n. */
    for (idx_t n = computed_n + 1; n <= upto; ++n) {
        /* --------- DP for width 6 --------- */
        for (uint32_t i = 0; i < STATE6; ++i) dp6_next[i] = 0;
        for (uint32_t mask = 0; mask < STATE6; ++mask) {
            seq_t val = dp6[mask];
            if (!val) continue;
            unsigned bits = __builtin_popcount(mask);

            /* Add element to one of the existing blocks */
            if (bits) {
                uint32_t new_mask = (mask << 1) & MASK6_KEEP;
                dp6_next[new_mask] += val * (seq_t)bits;
            }

            /* Start a new block at this position */
            uint32_t new_mask2 = ((mask << 1) & MASK6_KEEP) | 1U;
            dp6_next[new_mask2] += val;
        }
        /* swap buffers */
        seq_t *tmp = dp6; dp6 = dp6_next; dp6_next = tmp;

        /* Sum over all states to get total partitions for this n */
        seq_t sum6 = 0;
        for (uint32_t i = 0; i < STATE6; ++i) sum6 += dp6[i];
        arr6[n] = sum6;

        /* --------- DP for width 5 --------- */
        for (uint32_t i = 0; i < STATE5; ++i) dp5_next[i] = 0;
        for (uint32_t mask = 0; mask < STATE5; ++mask) {
            seq_t val = dp5[mask];
            if (!val) continue;
            unsigned bits = __builtin_popcount(mask);

            if (bits) {
                uint32_t new_mask = (mask << 1) & MASK5_KEEP;
                dp5_next[new_mask] += val * (seq_t)bits;
            }

            uint32_t new_mask2 = ((mask << 1) & MASK5_KEEP) | 1U;
            dp5_next[new_mask2] += val;
        }
        tmp = dp5; dp5 = dp5_next; dp5_next = tmp;

        seq_t sum5 = 0;
        for (uint32_t i = 0; i < STATE5; ++i) sum5 += dp5[i];
        arr5[n] = sum5;
    }

    computed_n = upto;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 6)                     /* offset is 6; behaviour below is irrelevant */
        return (seq_t)0;

    ensure_precomputed(n);
    return arr6[n] - arr5[n];
}
