/*
 * A261141  Positive integers n that can be expressed as a product of
 *          Jacobsthal numbers (A001045), not necessarily distinct.
 * https://oeis.org/A261141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 368, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261141: positive integers that can be expressed as a product of
   Jacobsthal numbers (A001045).  The algorithm generates the terms in
   increasing order by maintaining a min‑heap of the currently smallest
   products for each Jacobsthal generator.  Each generator is the value
   J(k) for k ≥ 3; the first generator is 3.  The heap always contains
   exactly one candidate per generator.  When a value is removed from the
   heap it is appended to the sequence and the corresponding generator
   index is advanced to its next multiplicative combination.  Duplicate
   values are eliminated by repeatedly popping equal values from the heap. */

#include <stdlib.h>

typedef idx_t idx_t;
typedef seq_t seq_t;

#define MAX_GEN 70   /* number of Jacobsthal generators to pre‑compute */

/* Node in the min‑heap: smallest product for a given generator */
struct Node {
    seq_t val;     /* current product */
    size_t gen;    /* index of generator in generators[] */
};

/* Global state (memoised between calls) */
static int initialized = 0;
static seq_t generators[MAX_GEN];
static size_t gen_count;
static size_t *pos;                /* pos[gen] = index in seq[] used for generator */
static struct Node *heap;          /* 1‑based heap of size gen_count */
static size_t heap_size;
static seq_t *seq;                 /* array of generated sequence values */
static size_t seq_len;
static size_t seq_cap;

/* Helper function: swap two heap nodes */
static inline void heap_swap(size_t i, size_t j)
{
    struct Node tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

/* Heap sift‑up from position idx */
static void heap_sift_up(size_t idx)
{
    while (idx > 1) {
        size_t parent = idx >> 1;
        if (heap[parent].val <= heap[idx].val)
            break;
        heap_swap(parent, idx);
        idx = parent;
    }
}

/* Heap sift‑down from position idx */
static void heap_sift_down(size_t idx)
{
    while (1) {
        size_t left  = idx << 1;
        if (left > heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right <= heap_size && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[smallest].val < heap[idx].val) {
            heap_swap(idx, smallest);
            idx = smallest;
        } else break;
    }
}

/* Insert a new node into the heap */
static void heap_push(size_t gen, seq_t val)
{
    heap[++heap_size].gen = gen;
    heap[heap_size].val = val;
    heap_sift_up(heap_size);
}

/* Remove and return the minimal node from the heap */
static struct Node heap_pop_min(void)
{
    struct Node res = heap[1];
    heap[1] = heap[heap_size--];
    heap_sift_down(1);
    return res;
}

/* Initialise global data on first call */
static void initialise(void)
{
    /* Compute Jacobsthal numbers J(k), k >= 3, skipping values <= 1 */
    seq_t j0 = 0, j1 = 1;      /* J(0), J(1) */
    size_t count = 0;
    while (count < MAX_GEN) {
        seq_t j = j1 + (seq_t)2 * j0;  /* J(n) = J(n-1) + 2*J(n-2) */
        if (j > 1) {
            generators[count++] = j;
        }
        j0 = j1;
        j1 = j;
    }
    gen_count = count;

    /* Allocate per‑generator positions array */
    pos = (size_t *)malloc(gen_count * sizeof(size_t));
    for (size_t i = 0; i < gen_count; ++i)
        pos[i] = 0;

    /* Allocate and initialise heap */
    heap = (struct Node *)malloc((gen_count + 1) * sizeof(struct Node)); /* 1‑based */
    heap_size = 0;
    for (size_t i = 0; i < gen_count; ++i)
        heap_push(i, generators[i]);   /* seq[0] = 1 (will be added later) */

    /* Allocate initial sequence array and set first term to 1 */
    seq_cap = 64;
    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    seq_len = 0;
    seq[seq_len++] = (seq_t)1;  /* a(1) = 1 */
    initialized = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized)
        initialise();

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        /* Extract smallest candidate from heap */
        struct Node cur = heap_pop_min();
        seq_t val = cur.val;
        size_t g   = cur.gen;

        /* Append to sequence */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        }
        seq[seq_len++] = val;

        /* Advance generator g to next position and push new candidate */
        ++pos[g];
        seq_t new_val = generators[g] * seq[pos[g]];
        heap_push(g, new_val);

        /* Remove duplicates: all heap entries equal to current value */
        while (heap_size >= 1 && heap[1].val == val) {
            struct Node dup = heap_pop_min();
            size_t dg = dup.gen;
            ++pos[dg];
            seq_t dv = generators[dg] * seq[pos[dg]];
            heap_push(dg, dv);
        }
    }

    /* n is 1‑based, seq array is 0‑based */
    return seq[(size_t)n - 1];
}
