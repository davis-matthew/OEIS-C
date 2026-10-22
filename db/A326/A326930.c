/*
 * A326930  Height of the smallest stack in a block-stacking sequence.
 * https://oeis.org/A326930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Height of the smallest stack in the block‑stacking process.
   a(1) = 1.
   For n > 1 let m be the current minimum stack height.
   If n <= m, create a new stack of height n.
   Otherwise, one stack of height m receives n blocks, becoming m + n.
   a(n) is the height of the smallest stack after step n. */
#include <stdlib.h>

/* maximum number of stacks that may appear for n <= 10000;
   use a fixed size array to avoid realloc which would
   break for big‑integer seq_t types. */
#define MAX_STACKS 20000

static seq_t heap[MAX_STACKS];   /* min‑heap of current stack heights */
static size_t heap_len = 0;      /* number of elements in the heap */

/* push a value onto the min‑heap */
static void heap_push(seq_t val)
{
    size_t idx = heap_len++;
    heap[idx] = val;
    /* bubble up */
    while (idx) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent] <= heap[idx]) break;
        seq_t tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx] = tmp;
        idx = parent;
    }
}

/* remove and return the minimum element of the heap */
static seq_t heap_pop(void)
{
    seq_t top = heap[0];
    heap[0] = heap[--heap_len];
    size_t idx = 0;
    while (1) {
        size_t left  = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;
        if (left < heap_len && heap[left] < heap[smallest]) smallest = left;
        if (right < heap_len && heap[right] < heap[smallest]) smallest = right;
        if (smallest == idx) break;
        seq_t tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
    return top;
}

/* current minimum stack height (heap is non‑empty) */
static inline seq_t heap_min(void)
{
    return heap[0];
}

/* compute the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    /* reset heap for each independent call; O(n log n) is fine for n <= 10000 */
    heap_len = 0;
    heap_push((seq_t)1);           /* a(1) = 1 */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur_min = heap_min();
        if ((seq_t)i <= cur_min) {
            heap_push((seq_t)i);
        } else {
            seq_t smallest = heap_pop();
            heap_push(smallest + (seq_t)i);
        }
    }
    return heap_min();
}
