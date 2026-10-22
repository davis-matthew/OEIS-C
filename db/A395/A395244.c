/*
 * A395244  Numbers which are the Collatz iteration encoding of some Collatz
 *          trajectory.
 * https://oeis.org/A395244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A395244: numbers c for which the map D(c) is defined
   according to the Collatz‑encoding rules. The function generates terms
   in increasing order by exploring the binary tree of allowed steps. */

#include <stdlib.h>
#include <stdbool.h>

/* Node stores a value `c` and its corresponding D(c). */
struct Node {
    seq_t c;
    seq_t d;
};

static struct Node *heap = NULL;
static size_t heap_sz = 0, heap_cap = 0;

static seq_t *seq_arr = NULL;
static size_t seq_len = 0, seq_cap = 0;

/* Push a node onto the min‑heap. */
static void heap_push(seq_t c, seq_t d)
{
    if (heap_sz == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        struct Node *tmp = (struct Node*)realloc(heap, new_cap * sizeof(struct Node));
        heap = tmp;
        heap_cap = new_cap;
    }
    size_t i = heap_sz++;
    heap[i].c = c;
    heap[i].d = d;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].c <= heap[i].c) break;
        struct Node tmp = heap[p];
        heap[p] = heap[i];
        heap[i] = tmp;
        i = p;
    }
}

/* Pop the smallest node from the min‑heap. */
static struct Node heap_pop(void)
{
    struct Node res = heap[0];
    --heap_sz;
    if (heap_sz) {
        heap[0] = heap[heap_sz];
        size_t i = 0;
        while (1) {
            size_t l = i * 2 + 1;
            if (l >= heap_sz) break;
            size_t r = l + 1;
            size_t child = (r < heap_sz && heap[r].c < heap[l].c) ? r : l;
            if (heap[i].c <= heap[child].c) break;
            struct Node tmp = heap[i];
            heap[i] = heap[child];
            heap[child] = tmp;
            i = child;
        }
    }
    return res;
}

/* Make sure the sequence array contains at least `n` terms. */
static void ensure_terms(idx_t n)
{
    if (seq_len >= (size_t)n) return;

    /* Allocate storage for sequence terms if not yet done. */
    if (seq_arr == NULL) {
        seq_cap = 128;
        seq_arr = (seq_t*)realloc(NULL, seq_cap * sizeof(seq_t));
    }

    /* Allocate storage for heap and insert the root if not yet done. */
    if (heap == NULL) {
        heap_cap = 16;
        heap = (struct Node*)realloc(NULL, heap_cap * sizeof(struct Node));
        heap_sz = 0;
        heap_push((seq_t)0, (seq_t)1);      /* D(0) = 1 */
    }

    while (seq_len < (size_t)n) {
        if (seq_len >= seq_cap) {
            size_t new_cap = seq_cap ? seq_cap * 2 : 128;
            seq_t *tmp = (seq_t*)realloc(seq_arr, new_cap * sizeof(seq_t));
            seq_arr = tmp;
            seq_cap = new_cap;
        }
        struct Node node = heap_pop();
        seq_arr[seq_len++] = node.c;

        seq_t c = node.c;
        seq_t d = node.d;

        /* Odd child: 2c + 1 with D = 2d */
        heap_push((c << 1) + (seq_t)1, d << 1);

        /* Even child exists only under the specified conditions */
        if (c > (seq_t)0 &&
            d > (seq_t)4 &&
            (d % (seq_t)6) == (seq_t)4) {
            seq_t c2 = c << 1;
            seq_t d2 = (d - (seq_t)1) / (seq_t)3;
            heap_push(c2, d2);
        }
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* safeguard for offset 1 */
    ensure_terms(n);
    return seq_arr[n - 1];
}
