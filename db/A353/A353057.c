/*
 * A353057  Numbers N of the form m^k in ascending order having the property
 *          that for any choice of m and k such that N = m^k, the sets of
 *          distinct digits of m, k, and m^k are pairwise disjoint.
 * https://oeis.org/A353057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Build the sequence by generating perfect powers in ascending order
   and keeping those for which every representation m^k uses pairwise
   disjoint sets of decimal digits.  The function is memoised, works
   for any seq_t size (8–128‑bit unsigned or arbitrary precision) and
   follows the sequence definition of OEIS A353057. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_digits.h"

typedef struct {
    seq_t val;          /* current value m^k */
    uint64_t base;      /* m */
    uint64_t exp;       /* k */
} node_t;

/* ---------- digit mask helper ------------------------------------ */
static inline unsigned digit_mask_seq(seq_t x)
{
    unsigned mask = 0;
    uint8_t buf[128];
    size_t len;
    get_digits(x, buf, &len);
    for (size_t i = 0; i < len; ++i)
        mask |= 1u << buf[i];
    return mask;
}

/* ---------- heap (min‑priority queue) -------------------------------- */
static void heap_push(node_t **pheap, size_t *plen, size_t *pcap, node_t val)
{
    node_t *heap = *pheap;
    if (*plen == *pcap) {
        size_t new_cap = (*pcap == 0) ? 64 : (*pcap << 1);
        node_t *tmp = (node_t *)realloc(heap, new_cap * sizeof(node_t));
        if (!tmp) exit(EXIT_FAILURE);
        heap = tmp;
        *pheap = heap;
        *pcap = new_cap;
    }
    heap[(*plen)++] = val;

    /* sift up */
    size_t i = *plen - 1;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[i].val < heap[p].val) {
            node_t t = heap[i]; heap[i] = heap[p]; heap[p] = t;
            i = p;
        } else break;
    }
}

static inline bool heap_empty(node_t *heap, size_t len) { return len == 0; }

static node_t heap_pop(node_t *heap, size_t *plen)
{
    node_t top = heap[0];
    heap[0] = heap[--(*plen)];
    size_t i = 0;
    while (1) {
        size_t l = i << 1 | 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < *plen && heap[l].val < heap[smallest].val) smallest = l;
        if (r < *plen && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        node_t t = heap[i]; heap[i] = heap[smallest]; heap[smallest] = t;
        i = smallest;
    }
    return top;
}

/* ---------- representation array helpers ----------------------------- */
static inline void rep_push(node_t **arr, size_t *len, size_t *cap, node_t val)
{
    if (*len == *cap) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        node_t *tmp = (node_t *)realloc(*arr, new_cap * sizeof(node_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
    (*arr)[(*len)++] = val;
}

/* ---------- sequence storage ---------------------------------------- */
static seq_t *terms = NULL;
static size_t terms_len = 0, terms_cap = 0;

/* ---------- heap storage ------------------------------------------- */
static node_t *heap = NULL;
static size_t heap_len = 0, heap_cap = 0;
static const uint64_t INITIAL_MAX_BASE = 10000;   /* enough for 10 000 terms */

/* ---------- main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of range, value unused */
    static idx_t max_needed = 0;
    if (n > max_needed) {
        max_needed = n;

        /* initialise terms storage once */
        if (terms_cap == 0) {
            terms_cap = 1024;
            terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
            if (!terms) exit(EXIT_FAILURE);
        }

        /* initialise heap once */
        if (heap_cap == 0) {
            heap_cap = 2048;
            heap = (node_t *)malloc(heap_cap * sizeof(node_t));
            if (!heap) exit(EXIT_FAILURE);

            /* push all squares m^2 for m in [2, INITIAL_MAX_BASE] */
            for (uint64_t m = 2; m <= INITIAL_MAX_BASE; ++m) {
                seq_t v = (seq_t)m * (seq_t)m;
                node_t nd = { v, m, 2 };
                heap_push(&heap, &heap_len, &heap_cap, nd);
            }
        }

        /* generate terms until we have enough */
        while (terms_len < (size_t)n) {
            if (heap_empty(heap, heap_len)) {
                /* extend the base range if needed */
                uint64_t old_limit = INITIAL_MAX_BASE;
                uint64_t new_limit = old_limit * 2;
                for (uint64_t m = old_limit + 1; m <= new_limit; ++m) {
                    seq_t v = (seq_t)m * (seq_t)m;
                    node_t nd = { v, m, 2 };
                    heap_push(&heap, &heap_len, &heap_cap, nd);
                }
                continue;   /* re‑enter while loop after expansion */
            }

            /* pop smallest value and collect all equal representations */
            node_t first = heap_pop(heap, &heap_len);
            seq_t val = first.val;

            node_t *repr = NULL;
            size_t repr_len = 0, repr_cap = 0;
            rep_push(&repr, &repr_len, &repr_cap, first);

            while (!heap_empty(heap, heap_len) && heap[0].val == val) {
                rep_push(&repr, &repr_len, &repr_cap, heap_pop(heap, &heap_len));
            }

            /* test validity */
            bool valid = true;
            unsigned maskN = digit_mask_seq(val);
            for (size_t i = 0; i < repr_len; ++i) {
                unsigned maskM = digit_mask_seq(repr[i].base);
                unsigned maskK = digit_mask_seq(repr[i].exp);
                if ((maskM & maskK) || (maskM & maskN) || (maskK & maskN)) {
                    valid = false;
                    break;
                }
            }

            if (valid) {
                if (terms_len == terms_cap) {
                    terms_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    terms = tmp;
                }
                terms[terms_len++] = val;
            }

            /* push successors of all popped representations */
            for (size_t i = 0; i < repr_len; ++i) {
                seq_t next_val = repr[i].val * (seq_t)repr[i].base;
                if (next_val > repr[i].val) { /* no overflow for uint64_t case */
                    node_t nxt = { next_val, repr[i].base, repr[i].exp + 1 };
                    heap_push(&heap, &heap_len, &heap_cap, nxt);
                }
            }
        }
    }
    return terms[n - 1];
}
