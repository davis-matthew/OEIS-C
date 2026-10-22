/*
 * A370759  Numbers expressible in the form k*m + 2*(k+m) - 1, for positive k
 *          and m.
 * https://oeis.org/A370759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A370759
   a(n) : smallest n-th integer that can be written as
          k*m + 2*(k+m) - 1 ,  with k,m ≥ 1
        = (k+2)*(m+2) - 5,  with k+2, m+2 ≥ 3
   The sequence consists of all numbers of the form a*b-5
   where a,b ≥ 3, sorted increasingly without duplicates.
   Implementation: a min‑heap over (i,j) pairs (i=j≥3) that yields
   the values i*j-5 in ascending order. Duplicate values are skipped.
   Runtime: O(n log n) with small constant factor; fast for n≈10⁴.
*/

#include <stdbool.h>
#include <stdlib.h>

/* Helper structure for the priority queue. */
typedef struct {
    seq_t prod;   /* a*b   */
    idx_t i, j;   /* indices (a,b) with a=i, b=j */
} HeapNode;

/* Global dynamic heap. */
static HeapNode *heap = NULL;
static size_t heap_cnt = 0;
static size_t heap_cap = 0;

/* Global cache of computed sequence values. */
static seq_t *seq_cache = NULL;
static size_t seq_size = 0;
static size_t seq_cap = 0;

/* Tiny dynamic array helpers ------------------------------------------------ */

static void heap_init(void)
{
    heap_cap = 8;
    heap = (HeapNode *)malloc(heap_cap * sizeof(HeapNode));
    heap_cnt = 0;
}

static void heap_push(HeapNode node)
{
    if (heap_cnt >= heap_cap) {
        heap_cap <<= 1;
        heap = (HeapNode *)realloc(heap, heap_cap * sizeof(HeapNode));
    }
    size_t idx = heap_cnt++;
    heap[idx] = node;
    /* sift up */
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].prod <= heap[idx].prod) break;
        HeapNode tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

static void heap_pop(HeapNode *out)
{
    *out = heap[0];
    heap[0] = heap[--heap_cnt];
    size_t idx = 0;
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_cnt) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_cnt && heap[right].prod < heap[left].prod)
            smallest = right;
        if (heap[smallest].prod >= heap[idx].prod) break;
        HeapNode tmp = heap[smallest];
        heap[smallest] = heap[idx];
        heap[idx] = tmp;
        idx = smallest;
    }
}

static void seq_grow(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Main sequence function ----------------------------------------------------- */

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        heap_init();
        /* push initial diagonal start (i=3,j=3) */
        HeapNode n0;
        n0.prod = (seq_t)3 * (seq_t)3;
        n0.i = 3;
        n0.j = 3;
        heap_push(n0);
    }

    /* Extend cache until we have at least n terms */
    while (seq_size < n) {
        HeapNode cur;
        heap_pop(&cur);
        seq_t val = cur.prod - 5;      /* a*b - 5  */

        /* Store if not a duplicate of the previous value */
        if (seq_size == 0 || val != seq_cache[seq_size - 1]) {
            seq_grow(seq_size + 1);
            seq_cache[seq_size++] = val;
        }

        /* push successor along the same row (i fixed, j+1) */
        HeapNode next;
        next.prod = (seq_t)cur.i * (seq_t)(cur.j + 1);
        next.i = cur.i;
        next.j = cur.j + 1;
        heap_push(next);

        /* When the first element of a row is popped, start the next row */
        if (cur.j == cur.i) {
            HeapNode new_start;
            new_start.prod = (seq_t)(cur.i + 1) * (seq_t)(cur.i + 1);
            new_start.i = cur.i + 1;
            new_start.j = cur.i + 1;
            heap_push(new_start);
        }
    }
    return seq_cache[n - 1];
}
