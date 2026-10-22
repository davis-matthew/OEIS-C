/*
 * A349062  Powerful numbers (A001694) with a record gap to the next powerful
 *          number.
 * https://oeis.org/A349062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 48, u32 n <= 8740, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A349062 – Powerful numbers (A001694) with a record gap to the next powerful number.
   The function generates powerful numbers in increasing order, keeps track of the gaps
   between consecutive powerful numbers, and records the powerful number that
   precedes a new record gap.  a(1) is the first term of the OEIS sequence.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"

/* power‑number entry in the heap: n = a² * b³ */
typedef struct {
    seq_t val;   /* value a² * b³ */
    uint64_t a;  /* current a  (≥1) */
    uint64_t b;  /* current b  (≥1) */
    seq_t b3;    /* pre‑computed b³ */
} node_t;

/* ---------- simple binary min‑heap for node_t -------------------------------- */
static void heap_init(node_t **h, size_t *len, size_t *cap)
{
    *len = 0;
    *cap = 16;
    *h   = (node_t *)malloc(*cap * sizeof(node_t));
}

static void heap_expand(node_t **h, size_t *cap)
{
    *cap = (*cap << 1) + 1;
    *h   = (node_t *)realloc(*h, *cap * sizeof(node_t));
}

static void heap_push(node_t **h, size_t *len, size_t *cap, node_t val)
{
    if (*len == *cap) heap_expand(h, cap);
    size_t i = *len;
    (*h)[i]  = val;
    (*len)++;
    /* bubble up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if ((*h)[p].val <= (*h)[i].val) break;
        node_t tmp = (*h)[i];
        (*h)[i]   = (*h)[p];
        (*h)[p]   = tmp;
        i = p;
    }
}

static node_t heap_pop(node_t **h, size_t *len)
{
    node_t top = (*h)[0];
    (*len)--;
    if (*len == 0) return top;
    node_t last = (*h)[*len];
    size_t i = 0;
    for (;;) {
        size_t l = (i << 1) + 1;
        if (l >= *len) break;
        size_t r = l + 1;
        size_t best = (r < *len && (*h)[r].val < (*h)[l].val) ? r : l;
        if (last.val <= (*h)[best].val) break;
        (*h)[i] = (*h)[best];
        i = best;
    }
    (*h)[i] = last;
    return top;
}

/* ---------- sequence routine ---------------------------------------------- */
seq_t a(idx_t n)
{
    /* static structures to maintain state between calls */
    static seq_t *records = NULL;          /* record–holder powerful numbers */
    static size_t rec_len = 0, rec_cap = 0;

    static node_t *heap = NULL;            /* min‑heap of powers */
    static size_t heap_len = 0, heap_cap = 0;
    static uint64_t Bmax = 0;             /* largest b pushed so far */
    static seq_t max_gap = 0;              /* current maximum gap found */
    static seq_t prev_unique = 0;          /* last unique powerful number */
    static seq_t last_popped = 0;          /* last popped value (used to filter duplicates) */

    /* lazy allocations */
    if (records == NULL) {
        rec_cap   = 16;
        records   = (seq_t *)malloc(rec_cap * sizeof(seq_t));
    }
    if (heap == NULL) {
        heap_cap = 16;
        heap     = (node_t *)malloc(heap_cap * sizeof(node_t));
    }

    /* generate powerful numbers until we have at least n record holders */
    while (rec_len < n) {
        /* initialise: push the very first powerful number (b = 1, a = 1) */
        if (heap_len == 0) {
            Bmax = 1;
            seq_t b3 = (seq_t)Bmax * Bmax * Bmax;
            node_t nd = {b3, 1, Bmax, b3};
            heap_push(&heap, &heap_len, &heap_cap, nd);
        }

        /* ensure all b with b³ ≤ current smallest value are present */
        while (heap_len > 0) {
            seq_t v = heap[0].val;
            idx_t need = (idx_t)floor_cube_root(v);
            if (Bmax >= need) break;
            Bmax++;
            seq_t b3 = (seq_t)Bmax * Bmax * Bmax;
            node_t nd = {b3, 1, Bmax, b3};
            heap_push(&heap, &heap_len, &heap_cap, nd);
        }

        /* pop the next powerful number */
        node_t cur_node = heap_pop(&heap, &heap_len);
        seq_t cur_val = cur_node.val;

        /* skip duplicate values */
        if (cur_val == last_popped) {
            /* push successor in the same b‑stream */
            uint64_t next_a = cur_node.a + 1;
            seq_t val_next = cur_node.b3 * (seq_t)next_a * (seq_t)next_a;
            node_t next_node = {val_next, next_a, cur_node.b, cur_node.b3};
            heap_push(&heap, &heap_len, &heap_cap, next_node);
            last_popped = cur_val;
            continue;
        }

        /* we have found a unique powerful number */
        last_popped = cur_val;

        /* compute gap to this new powerful number */
        if (prev_unique != 0) {
            seq_t gap = cur_val - prev_unique;
            if (gap > max_gap) {
                max_gap = gap;
                /* store the powerful number that precedes the new record gap */
                if (rec_len == rec_cap) {
                    rec_cap <<= 1;
                    records = (seq_t *)realloc(records, rec_cap * sizeof(seq_t));
                }
                records[rec_len++] = prev_unique;
            }
        }

        /* update history */
        prev_unique = cur_val;

        /* push the successor in the same b‑stream */
        uint64_t next_a = cur_node.a + 1;
        seq_t val_next = cur_node.b3 * (seq_t)next_a * (seq_t)next_a;
        node_t next_node = {val_next, next_a, cur_node.b, cur_node.b3};
        heap_push(&heap, &heap_len, &heap_cap, next_node);
    }

    /* a(1) is stored at records[0] */
    return records[n - 1];
}
