/*
 * A336531  A sieve: start with the positive integers. Let a(1)=1. Mark out
 *          the following numbers: a(1)+1, a(1)+1+2, a(1)+1+2+3, a(1)+1+2+3+4,
 *          ... . The next integer in the list not marked out is 3, so a(2)=3.
 *          Mark out the following numbers: a(2)+1, a(2)+1+2, a(2)+1+2+3,
 *          a(2)+1+2+3+4, ... . Repeat the proced...
 * https://oeis.org/A336531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A336531: sieve-like construction
   a(1) = 1; afterwards
   for each selected a(k) add to the marked set
   a(k)+1, a(k)+1+2, a(k)+1+2+3, … .
   The next unmarked integer becomes a(k+1). */

#include <stdlib.h>
#include <stdbool.h>

/* Heap node storing the next marked position from a particular a(k)
   and the current triangular index. */
struct HeapNode {
    seq_t pos;      /* next number that will be marked */
    idx_t k;        /* current triangular step (k ≥ 1) */
};

static struct HeapNode *heap = NULL;   /* dynamic heap array */
static size_t heap_cap = 0, heap_sz = 0;

/* push a new node into the min‑heap */
static void heap_push(seq_t pos, idx_t k)
{
    if (heap_sz == heap_cap) {
        size_t newcap = heap_cap ? heap_cap << 1 : 16;
        struct HeapNode *tmp = (struct HeapNode *)
            realloc(heap, newcap * sizeof *tmp);
        if (!tmp) exit(EXIT_FAILURE);
        heap = tmp;
        heap_cap = newcap;
    }
    size_t i = heap_sz++;
    heap[i].pos = pos;
    heap[i].k   = k;
    /* sift up */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].pos <= heap[i].pos) break;
        struct HeapNode tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

/* pop the smallest element from the heap */
static struct HeapNode heap_pop(void)
{
    struct HeapNode root = heap[0];
    --heap_sz;
    if (heap_sz) {
        heap[0] = heap[heap_sz];
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1, r = l + 1;
            size_t smallest = i;
            if (l < heap_sz && heap[l].pos < heap[smallest].pos) smallest = l;
            if (r < heap_sz && heap[r].pos < heap[smallest].pos) smallest = r;
            if (smallest == i) break;
            struct HeapNode tmp = heap[i];
            heap[i] = heap[smallest];
            heap[smallest] = tmp;
            i = smallest;
        }
    }
    return root;
}

/* storage for the sequence a(n) */
static seq_t *seq = NULL;
static size_t seq_cap = 0, seq_len = 0;
/* next integer to examine */
static seq_t cursor = (seq_t)1;

/* append a value to seq, resizing as needed */
static void seq_push(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t newcap = seq_cap ? seq_cap << 1 : 16;
        seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof *tmp);
        if (!tmp) exit(EXIT_FAILURE);
        seq = tmp;
        seq_cap = newcap;
    }
    seq[seq_len++] = val;
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, but harmless */

    while (seq_len < n) {
        seq_t candidate = cursor;
        bool marked = false;

        /* process all pending marks that are ≤ candidate */
        while (heap_sz > 0 && heap[0].pos <= candidate) {
            struct HeapNode node = heap_pop();
            if (node.pos == candidate) marked = true;
            /* schedule next mark from this a(k) */
            seq_t newpos = node.pos + ((seq_t)node.k + (seq_t)1);
            heap_push(newpos, node.k + 1);
        }

        if (!marked) {
            /* candidate is the next a(k) */
            seq_push(candidate);
            /* first mark for this a(k) is candidate+1 with step 1 */
            heap_push(candidate + (seq_t)1, (idx_t)1);
        }

        /* advance cursor to the next unchecked integer */
        cursor = candidate + (seq_t)1;
    }

    return seq[n - 1];
}
