/*
 * A342816  Numbers of the form (2^(2*j + 6*k + 10) - 2^(2*j + 2) - 3)/9, with
 *          j,k >= 0.
 * https://oeis.org/A342816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers of the form (2^(2*j + 6*k + 10) - 2^(2*j + 2) - 3)/9 with j,k >= 0 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"

/* Node in the priority queue: j and k give the pair, val is the sequence value */
typedef struct {
    seq_t val;
    uint64_t j;
    uint64_t k;
} node_t;

/* Compare two nodes: first by val, then by j and k to break ties */
static int node_cmp(const node_t *a, const node_t *b)
{
    if (a->val < b->val) return -1;
    if (a->val > b->val) return 1;
    if (a->j < b->j) return -1;
    if (a->j > b->j) return 1;
    if (a->k < b->k) return -1;
    if (a->k > b->k) return 1;
    return 0;
}

/* Sift up operation for the binary heap */
static void sift_up(node_t *heap, size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (node_cmp(&heap[idx], &heap[parent]) < 0) {
            node_t tmp = heap[idx];
            heap[idx] = heap[parent];
            heap[parent] = tmp;
            idx = parent;
        } else {
            break;
        }
    }
}

/* Sift down operation for the binary heap */
static void sift_down(node_t *heap, size_t idx, size_t heap_size)
{
    while (1) {
        size_t left = (idx << 1) + 1;
        size_t right = left + 1;
        size_t smallest = idx;

        if (left < heap_size && node_cmp(&heap[left], &heap[smallest]) < 0)
            smallest = left;
        if (right < heap_size && node_cmp(&heap[right], &heap[smallest]) < 0)
            smallest = right;

        if (smallest != idx) {
            node_t tmp = heap[idx];
            heap[idx] = heap[smallest];
            heap[smallest] = tmp;
            idx = smallest;
        } else {
            break;
        }
    }
}

/* Compute a single value for given j and k */
static seq_t compute_val(uint64_t j, uint64_t k)
{
    idx_t exp1 = (idx_t)(2 * j + 6 * k + 10);
    idx_t exp2 = (idx_t)(2 * j + 2);
    seq_t v1 = pow2(exp1);
    seq_t v2 = pow2(exp2);
    seq_t tmp = v1 - v2 - 3;
    return tmp / 9;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* not needed by specification */

    /* We will push j = 0 .. n-1 as initial entries.
       This guarantees that we can extract the first n terms. */
    size_t j_max = (size_t)n;
    size_t heap_size = j_max + 1;
    node_t *heap = (node_t *)malloc(heap_size * sizeof(node_t));
    if (!heap) return 0; /* allocation failure – unlikely for our size */

    /* Build initial heap with all j, k=0 */
    for (size_t j = 0; j <= j_max; ++j) {
        heap[j].j = j;
        heap[j].k = 0;
        heap[j].val = compute_val(j, 0);
    }
    /* Transform into a binary heap */
    for (size_t i = heap_size / 2; i > 0; --i)
        sift_down(heap, i - 1, heap_size);

    seq_t result = 0;

    /* Extract the smallest n terms, inserting the next k for each extracted j */
    for (idx_t count = 1; count <= n; ++count) {
        node_t cur = heap[0];
        result = cur.val;

        /* Move last element to root and shrink the heap */
        heap[0] = heap[--heap_size];
        sift_down(heap, 0, heap_size);

        /* Insert the next element with same j (k+1) */
        node_t new_node;
        new_node.j = cur.j;
        new_node.k = cur.k + 1;
        new_node.val = compute_val(cur.j, cur.k + 1);

        heap[heap_size++] = new_node;
        sift_up(heap, heap_size - 1);
    }

    free(heap);
    return result;
}
