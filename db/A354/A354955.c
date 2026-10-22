/*
 * A354955  Perfect powers j^k, such that k is the exponent of 2 in the
 *          factorization of j^k.
 * https://oeis.org/A354955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * A354956(n)
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   max_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of sequence A354955:
   Perfect powers j^k with k ≥ 2, j ≡ 2 (mod 4), i.e. j has a single factor 2.
   The sequence is generated in ascending order by repeatedly extracting the minimal
   candidate from a min‑heap and pushing the next power of the same base and the next
   base itself (j^2). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/max_seq_t.h"

/* Candidate value in the heap */
struct Candidate {
    seq_t val;          /* current power j^k */
    idx_t j;            /* base j (≡2 (mod4)) */
    unsigned int k;     /* current exponent k (starting at 2) */
};

static struct Candidate *heap = NULL;
static size_t heap_size = 0, heap_cap = 0;

static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;

static idx_t next_j = 2;          /* next base to insert (≡2 mod4) */

/* Heap operations: min‑heap on .val */
static void sift_up(size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].val <= heap[idx].val) break;
        struct Candidate tmp = heap[parent];
        heap[parent] = heap[idx];
        heap[idx]   = tmp;
        idx = parent;
    }
}

static void sift_down(size_t idx) {
    while (1) {
        size_t left   = (idx << 1) | 1;
        size_t right  = left + 1;
        size_t smallest = idx;

        if (left < heap_size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val)
            smallest = right;
        if (smallest == idx) break;

        struct Candidate tmp = heap[smallest];
        heap[smallest] = heap[idx];
        heap[idx]     = tmp;
        idx = smallest;
    }
}

static void heap_push(seq_t v, idx_t j, unsigned int k) {
    if (heap_size == heap_cap) {
        heap_cap = (heap_cap == 0) ? 16 : heap_cap * 2;
        heap = (struct Candidate *)realloc(heap, heap_cap * sizeof(struct Candidate));
    }
    heap[heap_size].val = v;
    heap[heap_size].j   = j;
    heap[heap_size].k   = k;
    sift_up(heap_size++);
}

static struct Candidate heap_pop(void) {
    struct Candidate top = heap[0];
    heap[0] = heap[--heap_size];
    sift_down(0);
    return top;
}

static void seq_append(seq_t v) {
    if (seq_len == seq_cap) {
        seq_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_len++] = v;
}

/* Initialise the heap with the first base (j=2) */
static void init_sequence(void) {
    seq_t val = (seq_t)next_j * (seq_t)next_j; /* j^2 for j=2 */
    heap_push(val, next_j, 2);
    next_j += 4;        /* next base will be 6 */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 0 */

    if (!heap) init_sequence();     /* first call initialisation */

    seq_t max_val = max_seq_t();

    while (seq_len < n) {
        /* Extract smallest candidate */
        struct Candidate cand = heap_pop();

        /* Append to sequence if not a duplicate (duplicates cannot occur) */
        if (seq_len == 0 || cand.val != seq[seq_len - 1])
            seq_append(cand.val);

        /* Push next power of the same base */
        unsigned __int128 prod =
            (unsigned __int128)cand.val * (unsigned __int128)cand.j;
        if (prod <= (unsigned __int128)max_val) {
            heap_push((seq_t)prod, cand.j, cand.k + 1);
        }

        /* Insert next base's square (j^2) if it fits */
        unsigned __int128 sq =
            (unsigned __int128)next_j * (unsigned __int128)next_j;
        if (sq <= (unsigned __int128)max_val) {
            heap_push((seq_t)sq, next_j, 2);
        }
        next_j += 4;            /* move to next base ≡ 2 mod 4 */
    }

    return seq[n - 1];
}
