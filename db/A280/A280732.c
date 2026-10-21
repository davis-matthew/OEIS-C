/*
 * A280732  Numbers whose digit string can be partitioned into three nonempty
 *          parts a <= b <= c such that a*b = c.
 * https://oeis.org/A280732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 624, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   digit_len, pow10, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Enumerate all triples (a,b,c) with a*b=c and a<=b,
   compute the concatenated number ((a|b)|c) and use a min‑heap
   to emit the values in increasing order.  The sequence is memoized
   in a dynamic array so that any call returns the unique nth term. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/seq_realloc.h"

#define A_LIMIT 200000ULL   /* number of a‑values pre‑injected into the heap */

/* Node stored in the heap */
typedef struct {
    seq_t N;     /* concatenated number a|b|c */
    idx_t a;     /* first part */
    idx_t b;     /* second part */
} HeapNode;

static HeapNode heap[A_LIMIT];
static size_t heap_size = 0;

static seq_t concat_ab_c(idx_t a, idx_t b)
{
    seq_t c = (seq_t)a * (seq_t)b;          /* c = a*b   */
    idx_t len_b = digit_len((seq_t)b);      /* digits of b  */
    idx_t len_c = digit_len(c);             /* digits of c  */
    seq_t N = (seq_t)a * pow10(len_b + len_c)
            + (seq_t)b * pow10(len_c)
            + c;
    return N;
}

/* ----- heap helpers ---------------------------------------------------- */
static void sift_up(size_t i)
{
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].N <= heap[i].N) break;
        HeapNode tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static void sift_down(size_t i)
{
    size_t left, right, smallest;
    while (true) {
        left  = (i << 1) + 1;
        right = left + 1;
        smallest = i;
        if (left  < heap_size && heap[left].N  < heap[smallest].N) smallest = left;
        if (right < heap_size && heap[right].N < heap[smallest].N) smallest = right;
        if (smallest == i) break;
        HeapNode tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void heap_push(HeapNode node)
{
    heap[heap_size++] = node;
    sift_up(heap_size - 1);
}

static HeapNode heap_pop(void)
{
    HeapNode root = heap[0];
    heap[0] = heap[--heap_size];
    sift_down(0);
    return root;
}

/* ----- dynamic array for memoization --------------------------------- */
static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;

static void seq_extend(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed) new_cap *= 2;
    seq = seq_realloc(seq, seq_cap, new_cap);
    seq_cap = new_cap;
}

/* ----- main function --------------------------------------------------- */
seq_t a(idx_t n)          /* external linkage required by the build   */
{
    static bool initialized = false;
    if (!initialized) {
        /* pre‑push the first A_LIMIT values of a with b==a */
        for (idx_t a_val = 1; a_val <= A_LIMIT; ++a_val) {
            HeapNode node = { .N = concat_ab_c(a_val, a_val),
                              .a = a_val,
                              .b = a_val };
            heap_push(node);
        }
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        HeapNode cur = heap_pop();

        /* store if new */
        seq_extend(seq_len + 1);
        if (seq_len == 0 || seq[seq_len - 1] != cur.N) {
            seq[seq_len++] = cur.N;
        }

        /* push the next candidate for this a (b+1) */
        idx_t next_b = cur.b + 1;
        HeapNode next = { .N = concat_ab_c(cur.a, next_b),
                          .a = cur.a,
                          .b = next_b };
        heap_push(next);
    }
    return seq[(size_t)n - 1];
}
