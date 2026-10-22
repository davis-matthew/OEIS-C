/*
 * A371602  Taxicab numbers that are sandwiched between squarefree numbers.
 * https://oeis.org/A371602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A371602: Taxicab numbers that are sandwiched between squarefree numbers.
 *
 * A(n) is the n‑th even integer that can be written as
 *   i³ + j³ = i³ + k³   (i≤j, k>j)
 * in at least two distinct unordered ways and whose neighbors (A(n)-1 and
 * A(n)+1) are squarefree.
 *
 * The algorithm generates all sums i³+j³ in ascending order using a
 * min‑heap.  Each heap element stores the current pair (i, j) to be
 * processed; for every popped element (i, j) a new element
 * (i, j+1) is pushed, thus generating all pairs with that i.
 *
 * A moderate upper bound for i (MAX_I) is chosen.  This is sufficient
 * to retrieve the first few thousand terms bounded by the run time
 * requirement; if additional terms are needed the bound should be
 * increased at compile time.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

typedef struct { seq_t sum; idx_t i; idx_t j; } Node;

/* ---------- simple binary heap (1‑based indexing) ---------- */
static void heap_push(Node *h, size_t *sz, const Node *v)
{
    size_t pos = ++(*sz);
    while (pos > 1) {
        size_t parent = pos >> 1;
        if (h[parent].sum <= v->sum) break;
        h[pos] = h[parent];
        pos = parent;
    }
    h[pos] = *v;
}

static Node heap_pop(Node *h, size_t *sz)
{
    Node res = h[1];
    Node last = h[(*sz)--];
    size_t pos = 1;
    while (true) {
        size_t left = pos << 1;
        if (left > *sz) break;
        size_t right = left + 1;
        size_t child = (right <= *sz && h[right].sum < h[left].sum) ? right : left;
        if (h[child].sum >= last.sum) break;
        h[pos] = h[child];
        pos = child;
    }
    h[pos] = last;
    return res;
}

/* ---------- main sequence function ---------- */
#define MAX_I 50000   /* adjust if more terms are required */

seq_t a(idx_t n)
{
    /* pre‑compute cubes */
    seq_t *cube = (seq_t*)malloc((MAX_I + 1) * sizeof(seq_t));
    for (idx_t i = 1; i <= MAX_I; ++i) {
        seq_t ii = (seq_t)i;
        cube[i] = ii * ii * ii;
    }

    /* allocate and initialise heap with (i,i) pairs */
    size_t heap_cap = MAX_I + 5;
    Node *heap = (Node*)malloc(heap_cap * sizeof(Node));
    size_t heap_sz = 0;
    for (idx_t i = 1; i <= MAX_I; ++i) {
        Node nd;
        nd.i = i; nd.j = i;
        nd.sum = cube[i] + cube[i];
        heap_push(heap, &heap_sz, &nd);
    }

    idx_t found = 0;
    seq_t prev_sum = 0;
    int reps = 0;
    bool first = true;
    seq_t answer = 0;

    while (true) {
        Node cur = heap_pop(heap, &heap_sz);
        seq_t s = cur.sum;

        if (first) {
            prev_sum = s; reps = 1; first = false;
        } else if (s == prev_sum) {
            ++reps;
        } else {
            if (reps >= 2 &&
                is_squarefree(prev_sum - 1) &&
                is_squarefree(prev_sum + 1)) {
                ++found;
                if (found == n) {
                    answer = prev_sum;
                    break;
                }
            }
            prev_sum = s; reps = 1;
        }

        /* push next pair for the same i */
        idx_t i = cur.i;
        idx_t j = cur.j + 1;
        Node nxt;
        nxt.i = i; nxt.j = j;
        nxt.sum = cube[i] + (seq_t)j * j * j;
        heap_push(heap, &heap_sz, &nxt);
    }

    free(heap);
    free(cube);
    return answer;
}
