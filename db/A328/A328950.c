/*
 * A328950  Numerators for the "Minimum-Redundancy Code" card problem.
 * https://oeis.org/A328950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): minimal total merge cost (Huffman optimal cost) for the
   multiset {1, 2, …, n}.  The function constructs a min‑heap and
   performs the standard Huffman merge procedure. */

#include <stdlib.h>
#include <stdbool.h>

static void sift_down(seq_t *heap, idx_t len, idx_t i)
{
    while (1) {
        idx_t left  = i * 2 + 1;
        idx_t right = left + 1;
        idx_t smallest = i;
        if (left < len && heap[left] < heap[smallest])
            smallest = left;
        if (right < len && heap[right] < heap[smallest])
            smallest = right;
        if (smallest == i)
            break;
        seq_t tmp = heap[i];
        heap[i]   = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

static void sift_up(seq_t *heap, idx_t i)
{
    while (i > 0) {
        idx_t parent = (i - 1) >> 1;
        if (heap[i] >= heap[parent])
            break;
        seq_t tmp = heap[i];
        heap[i]   = heap[parent];
        heap[parent] = tmp;
        i = parent;
    }
}

static seq_t pop_min(seq_t *heap, idx_t *len)
{
    seq_t val = heap[0];
    heap[0] = heap[--(*len)];
    sift_down(heap, *len, 0);
    return val;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;             /* a(1) = 0 */

    size_t cap = (size_t)n;          /* n is small in typical usage */
    seq_t *heap = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!heap)
        return (seq_t)0;             /* allocation failed, return 0 */

    for (idx_t i = 0; i < n; ++i)
        heap[i] = (seq_t)(i + 1);

    /* build min‑heap */
    for (idx_t i = n / 2; i-- > 0; ) {
        sift_down(heap, n, i);
    }

    seq_t total = (seq_t)0;
    idx_t len   = n;

    while (len > 1) {
        seq_t min1 = pop_min(heap, &len);
        seq_t min2 = pop_min(heap, &len);
        seq_t merged = min1 + min2;
        total += merged;
        heap[len++] = merged;        /* push back the merged weight */
        sift_up(heap, len - 1);
    }

    free(heap);
    return total;
}
