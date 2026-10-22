/*
 * A365808  Numbers k such that A163511(k) is a square.
 * https://oeis.org/A365808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=2; for every non‑zero term x, also 2*x+1 and 4*x
 *            are terms. The function returns the n‑th term (1‑indexed) by
 *            generating the sequence in increasing order using a min‑heap
 * Range:     u8 n <= 14, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array, heap_swap
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"
#include "../utils/heap_swap.h"

/* ---------- global static state ------------------------------------------- */

static seq_t *seq       = NULL;   /* the generated sequence */
static size_t seq_len   = 0;      /* number of terms currently stored */
static size_t seq_cap   = 0;      /* capacity of the seq array */

static seq_t *heap      = NULL;   /* min‑heap of pending candidates */
static size_t heap_len  = 0;      /* current number of elements in heap */
static size_t heap_cap  = 0;      /* capacity of the heap array */

static void heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent] <= heap[i]) break;
        heap_swap(&heap[parent], &heap[i]);
        i = parent;
    }
}

static void heap_sift_down(size_t i)
{
    while (1) {
        size_t left  = (i << 1) + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left  < heap_len && heap[left]  < heap[smallest]) smallest = left;
        if (right < heap_len && heap[right] < heap[smallest]) smallest = right;
        if (smallest == i) break;
        heap_swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
}

static void heap_push(seq_t val)
{
    if (heap_len == heap_cap) grow_seq_array(&heap, &heap_cap, heap_len + 1);
    heap[heap_len++] = val;
    heap_sift_up(heap_len - 1);
}

static seq_t heap_pop(void)
{
    seq_t res = heap[0];
    heap[0] = heap[--heap_len];
    heap_sift_down(0);
    return res;
}

/* ---------- public routine ----------------------------------------------- */

seq_t a(idx_t n)
{
    /* Initialise once */
    if (!seq) {
        seq_cap = 64;
        seq = alloc_seq_array(seq_cap);
        seq[0] = 0;  /* a(1) */
        seq[1] = 2;  /* a(2) */
        seq_len = 2;

        heap_cap = 64;
        heap = alloc_seq_array(heap_cap);
        heap_len = 0;

        /* push initial children of 2 (no children of 0) */
        heap_push((seq_t)5);   /* 2*2 + 1 */
        heap_push((seq_t)8);   /* 4*2   */
    }

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        seq_t val = heap_pop();

        if (seq_len == seq_cap) grow_seq_array(&seq, &seq_cap, seq_len + 1);
        seq[seq_len++] = val;

        if (val != 0) {          /* rule applies only to non‑zero terms */
            heap_push((seq_t)2 * val + 1);   /* 2*val + 1 */
            heap_push((seq_t)4 * val);       /* 4*val */
        }
    }

    return seq[n - 1];
}
