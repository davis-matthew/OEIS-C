/*
 * A272909  Numbers that are the product of two Lucas numbers L(i), for i >=
 *          1, using the Lucas numbers as defined in A000204.
 * https://oeis.org/A272909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth smallest product of two Lucas numbers L(i), i>=1
 *            (unordered pairs, product is L(i)*L(j), i<=j)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 320, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   lucas_overflow, mul_overflow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/lucas_overflow.h"
#include "../utils/mul_overflow.h"

typedef struct {
    seq_t val;
    size_t i, j;
} Node;

static void sift_up(Node heap[], size_t pos)
{
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].val <= heap[pos].val) break;
        Node tmp = heap[parent];
        heap[parent] = heap[pos];
        heap[pos] = tmp;
        pos = parent;
    }
}

static void sift_down(Node heap[], size_t pos, size_t heap_size)
{
    for (;;) {
        size_t left = (pos << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_size && heap[right].val < heap[left].val)
            smallest = right;
        if (heap[pos].val <= heap[smallest].val) break;
        Node tmp = heap[pos];
        heap[pos] = heap[smallest];
        heap[smallest] = tmp;
        pos = smallest;
    }
}

static void heap_push(Node heap[], size_t *heap_size, Node node)
{
    heap[*heap_size] = node;
    sift_up(heap, *heap_size);
    ++(*heap_size);
}

static Node heap_pop(Node heap[], size_t *heap_size)
{
    Node top = heap[0];
    --(*heap_size);
    if (*heap_size > 0) {
        heap[0] = heap[*heap_size];
        sift_down(heap, 0, *heap_size);
    }
    return top;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, but safe */
    /* Maximum number of Lucas numbers kept. 200 easily covers 10 000 terms. */
    const size_t MAX_LUCAS = 260;
    seq_t lucas[MAX_LUCAS];
    size_t lucas_count = 0;

    /* Initialize about the first two Lucas numbers (L(1)=1, L(2)=3). */
    lucas[0] = (seq_t)1;
    lucas[1] = (seq_t)3;
    lucas_count = 2;

    /* Generate further Lucas numbers while possible. */
    while (lucas_count < MAX_LUCAS) {
        seq_t nxt;
        if (!lucas_overflow(lucas[lucas_count - 1], lucas[lucas_count - 2], &nxt))
            break;                           /* would overflow seq_t */
        lucas[lucas_count++] = nxt;
    }

    /* Min‑heap for the products.  Each row i contributes at most one element. */
    Node heap[MAX_LUCAS];
    size_t heap_size = 0;

    /* Push the first product of each row (i,i).  Skip rows that overflow. */
    for (size_t i = 0; i < lucas_count; ++i) {
        seq_t prod;
        if (mul_overflow(lucas[i], lucas[i], &prod))
            heap_push(heap, &heap_size, (Node){prod, i, i});
    }

    seq_t answer = (seq_t)0;
    for (idx_t step = 1; step <= n; ++step) {
        /* If the heap becomes empty, generate a new Lucas number and start a new row. */
        while (heap_size == 0 && lucas_count < MAX_LUCAS) {
            seq_t nxt;
            if (!lucas_overflow(lucas[lucas_count - 1], lucas[lucas_count - 2], &nxt))
                break;
            lucas[lucas_count++] = nxt;
            seq_t prod;
            if (mul_overflow(lucas[lucas_count - 1], lucas[lucas_count - 1], &prod))
                heap_push(heap, &heap_size, (Node){prod, lucas_count - 1, lucas_count - 1});
        }

        if (heap_size == 0) break;   /* no more valid products */

        Node cur = heap_pop(heap, &heap_size);
        answer = cur.val;

        /* Advance in the same row: (i, j+1).  Skip if it overflows. */
        size_t next_j = cur.j + 1;
        if (next_j < lucas_count) {
            seq_t prod;
            if (mul_overflow(lucas[cur.i], lucas[next_j], &prod))
                heap_push(heap, &heap_size, (Node){prod, cur.i, next_j});
        }
    }
    return answer;
}
