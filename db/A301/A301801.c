/*
 * A301801  a(n) = smallest integer not yet in the sequence with no digits in
 *          common with a(n-1), a(n-2), a(n-3), and a(n-4); a(0)=0, a(1)=1,
 *          a(2)=2, a(3)=3.
 * https://oeis.org/A301801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A301801
   a(0)=0, a(1)=1, a(2)=2, a(3)=3
   a(n) = smallest unused integer such that its decimal digits have
   no overlap with the digits of a(n-1), a(n-2), a(n-3) and a(n-4). */

/* Digit mask compiler: bits 0..9 indicate presence of that decimal digit. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digit_counts.h"

/* Compute bit mask of decimal digits of x. */
static unsigned digit_mask(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    unsigned m = 0;
    for (int d = 0; d < 10; ++d)
        if (cnt[d]) m |= 1U << d;
    return m;
}

/* Bit‑set of values that have already appeared.  Index = integer value. */
static bool *used_bitmap = NULL;
static size_t used_cap = 0;

/* Ensure bitmap can index all values up to 'needed-1'. */
static void ensure_used_capacity(size_t needed)
{
    if (used_cap >= needed) return;
    size_t new_cap = used_cap ? used_cap : 64;
    while (new_cap < needed) new_cap <<= 1;
    bool *tmp = (bool *)realloc(used_bitmap, new_cap * sizeof(bool));
    if (!tmp) abort();
    /* New entries are false. */
    if (new_cap > used_cap)
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used_bitmap = tmp;
    used_cap = new_cap;
}

/* Storage for the sequence. */
static seq_t *seq = NULL;
static size_t seq_len = 0;
static size_t seq_cap = 0;

/* Grow the sequence storage. */
static void grow_seq(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* Masks of the last four terms, newest first. */
static unsigned last_masks[4] = {0, 0, 0, 0};

/* Initialise the first four terms once. */
static void init_sequence(void)
{
    if (seq != NULL) return;      /* already initialised */
    grow_seq(4);
    seq[0] = (seq_t)0;
    seq[1] = (seq_t)1;
    seq[2] = (seq_t)2;
    seq[3] = (seq_t)3;
    seq_len = 4;
    last_masks[0] = digit_mask(seq[3]);   /* a(3) */
    last_masks[1] = digit_mask(seq[2]);   /* a(2) */
    last_masks[2] = digit_mask(seq[1]);   /* a(1) */
    last_masks[3] = digit_mask(seq[0]);   /* a(0) */
    ensure_used_capacity(4);              /* indices 0..3 used */
    for (size_t i = 0; i < 4; ++i) used_bitmap[i] = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_sequence();

    /* Produce values up to index n. */
    while ((idx_t)seq_len <= n) {
        /* Combine the masks of the four most recent terms. */
        unsigned combined = last_masks[0] | last_masks[1] | last_masks[2] | last_masks[3];

        /* Search from the smallest integer upward for a suitable candidate. */
        uint64_t cand_val = 0;                /* 64‑bit temp for indexing */
        for (;;) {
            if (cand_val > SIZE_MAX) abort(); /* sanity check */
            size_t cand_idx = (size_t)cand_val;
            ensure_used_capacity(cand_idx + 1);

            if (!used_bitmap[cand_idx]) {
                seq_t cand = (seq_t)cand_val;
                unsigned m = digit_mask(cand);
                if ((m & combined) == 0) {
                    /* Found the next term. */
                    used_bitmap[cand_idx] = true;

                    /* Grow the sequence storage if needed. */
                    grow_seq(seq_len + 1);
                    seq[seq_len] = cand;

                    /* Update the masks for the next iteration. */
                    unsigned new_mask = m;
                    last_masks[3] = last_masks[2];
                    last_masks[2] = last_masks[1];
                    last_masks[1] = last_masks[0];
                    last_masks[0] = new_mask;

                    ++seq_len;
                    break;
                }
            }
            ++cand_val;
        }
    }

    return seq[(size_t)n];
}
