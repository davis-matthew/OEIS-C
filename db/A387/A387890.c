/*
 * A387890  First difference of consecutive even perfect powers (no perfect
 *          power between them).
 * https://oeis.org/A387890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A387890: first difference of consecutive even perfect powers
 * (no perfect power between them). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*---------------------------------------------------------------------*/
/*           Heap of next perfect powers (unique values).             */
/*---------------------------------------------------------------------*/

typedef struct {
    seq_t val;   /* current value a^b   */
    seq_t base;  /* base a              */
} Node;

/* dynamic min‑heap */
static Node *heap      = NULL;
static size_t heap_sz  = 0;
static size_t heap_cap = 0;
static idx_t  next_base_to_insert = 2;   /* next base not yet inserted */
static bool   heap_initialized = false;

/* swap two heap elements */
static inline void heap_swap(size_t i, size_t j)
{
    Node t = heap[i];
    heap[i] = heap[j];
    heap[j] = t;
}

/* ensure heap has space for an extra element */
static void ensure_heap_capacity(void)
{
    if (heap_sz >= heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 4;
        heap = (Node *)realloc(heap, heap_cap * sizeof(Node));
    }
}

/* push a node into the heap */
static void heap_push(Node n)
{
    ensure_heap_capacity();
    size_t i = heap_sz++;
    heap[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(p, i);
        i = p;
    }
}

/* pop the minimum element */
static Node heap_pop(void)
{
    Node top = heap[0];
    if (--heap_sz) {
        heap[0] = heap[heap_sz];
        size_t i = 0;
        for (;;) {
            size_t l = (i << 1) + 1;
            if (l >= heap_sz) break;
            size_t r = l + 1;
            size_t c = (r < heap_sz && heap[r].val < heap[l].val) ? r : l;
            if (heap[i].val <= heap[c].val) break;
            heap_swap(i, c);
            i = c;
        }
    }
    return top;
}

/* insert a base a with exponent 2 (its first power).  Return false
 * if the value overflows the seq_t range. */
static bool insert_base(idx_t a)
{
    seq_t v = (seq_t)a * (seq_t)a;
    if (v < (seq_t)a)    /* overflow detected for unsigned types */
        return false;
    heap_push((Node){v, (seq_t)a});
    return true;
}

/*---------------------------------------------------------------------*/
/*           Sequence generation – differences                       */
/*---------------------------------------------------------------------*/

static seq_t *diffs      = NULL;
static size_t diffs_sz   = 0;
static size_t diffs_cap  = 0;
static seq_t prev_cp     = 0;
static bool   prev_exists = false;

/* extend the difference array until it contains at least n terms */
static void extend_sequence(idx_t n)
{
    if (!heap_initialized) {
        heap_cap = (size_t)4;
        heap     = (Node *)malloc(heap_cap * sizeof(Node));
        heap_sz  = 0;

        diffs_cap = (size_t)64;
        diffs     = (seq_t *)malloc(diffs_cap * sizeof(seq_t));
        diffs_sz  = 0;

        prev_exists = false;
        next_base_to_insert = 2;
        heap_initialized = true;
    }

    while (diffs_sz < (size_t)n) {
        /* make sure the heap is non‑empty: at least base 2 */
        if (heap_sz == 0) {
            insert_base(next_base_to_insert);
            next_base_to_insert++;
        }

        /* insert all bases whose square does not exceed the current
         * smallest perfect power in the heap.  These are the only bases
         * that could produce a value smaller than or equal to the current
         * minimum – otherwise the base would have been inserted earlier. */
        seq_t min_val = heap[0].val;
        while (1) {
            seq_t sq = (seq_t)next_base_to_insert * (seq_t)next_base_to_insert;
            if (sq < (seq_t)next_base_to_insert)   /* overflow – stop */
                break;
            if (sq <= min_val) {
                insert_base(next_base_to_insert);
                next_base_to_insert++;
            } else
                break;
        }

        /* pop the next perfect power (unique value). */
        Node cur = heap_pop();
        seq_t val  = cur.val;
        seq_t base = cur.base;

        /* push the next power for this base, if it does not overflow. */
        seq_t next_val = val * base;
        if (next_val >= val && next_val >= base) {   /* no wrap‑around */
            heap_push((Node){next_val, base});
        }

        /* record a difference only if this value differs from the
         * previous perfect power and both are even. */
        if (prev_exists && val != prev_cp &&
            ((prev_cp & 1) == 0) && ((val & 1) == 0)) {
            seq_t diff = val - prev_cp;
            if (diffs_sz == diffs_cap) {
                diffs_cap *= 2;
                diffs = (seq_t *)realloc(diffs, diffs_cap * sizeof(seq_t));
            }
            diffs[diffs_sz++] = diff;
        }

        prev_cp     = val;
        prev_exists = true;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    extend_sequence(n);
    return diffs[n - 1];
}
