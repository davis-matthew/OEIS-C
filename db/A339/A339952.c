/*
 * A339952  Numbers that are the sum of an even square > 0 and an odd square.
 * https://oeis.org/A339952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 7671, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers that can be expressed as (2k)^2 + (2m+1)^2, with k≥1, m≥0
   (A339952).  The implementation builds the sequence by extracting the
   smallest sums from a min‑heap, memoising all generated terms.
   It works for any seq_t (builtin integer or arbitrary‑precision class)
   and for any idx_t (unsigned 64‑bit). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Choose a maximum index for k and m that comfortably covers the first
   ten thousand terms.  1024 is more than enough for the required range. */
#define MAX_IDX 1024

/* ---------- cache of generated terms ---------- */
static seq_t *seq_cache   = NULL;   /* cached values               */
static size_t seq_cap     = 0;      /* allocated capacity          */
static size_t seq_len     = 0;      /* number of values stored     */

/* ---------- min‑heap node ---------- */
typedef struct {
    seq_t  sum;   /* (2k)^2 + (2m+1)^2 */
    idx_t  i;     /* k  (≥1) */
    idx_t  j;     /* m  (≥0) */
} node_t;

/* ---------- heap storage ---------- */
static node_t *heap      = NULL;
static size_t  heap_cap  = 0;
static size_t  heap_len  = 0;

/* ---------- visited matrix to avoid duplicate pushes ----------
   We only ever consider indices < MAX_IDX, so a fixed array works. */
static bool visited[MAX_IDX][MAX_IDX] = {false};

/* ---------- helper: push a node onto the heap ---------- */
static void heap_push(seq_t sum, idx_t i, idx_t j)
{
    if (heap_len == heap_cap) {            /* grow the heap */
        size_t new_cap = heap_cap ? heap_cap * 2 : 64;
        node_t *new_heap = (node_t*)realloc(heap, new_cap * sizeof(node_t));
        if (!new_heap) abort();
        heap = new_heap;
        heap_cap = new_cap;
    }
    size_t pos = heap_len++;
    heap[pos].sum = sum;
    heap[pos].i   = i;
    heap[pos].j   = j;
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap[parent].sum <= heap[pos].sum) break;
        node_t tmp = heap[parent];
        heap[parent] = heap[pos];
        heap[pos] = tmp;
        pos = parent;
    }
}

/* ---------- helper: pop the smallest node ---------- */
static node_t heap_pop(void)
{
    node_t res = heap[0];
    heap_len--;
    if (heap_len == 0) return res;
    heap[0] = heap[heap_len];
    size_t pos = 0;
    while (true) {
        size_t left = (pos << 1) + 1;
        if (left >= heap_len) break;
        size_t right = left + 1;
        size_t smallest = left;
        if (right < heap_len && heap[right].sum < heap[left].sum)
            smallest = right;
        if (heap[pos].sum <= heap[smallest].sum) break;
        node_t tmp = heap[pos];
        heap[pos] = heap[smallest];
        heap[smallest] = tmp;
        pos = smallest;
    }
    return res;
}

/* ---------- ensure the cache contains at least N terms ----------
   The cache is grown lazily; terms are generated on demand. */
static void ensure_cache(idx_t N)
{
    if (seq_len >= (size_t)N) return;      /* already enough   */

    /* grow seq_cache if necessary */
    if (seq_cap < (size_t)N) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        while (new_cap < (size_t)N) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        seq_cache = new_cache;
        seq_cap   = new_cap;
    }

    /* initialise heap on the first call only */
    static bool first = true;
    if (first) {
        first = false;
        heap_cap = 64;
        heap = (node_t*)malloc(heap_cap * sizeof(node_t));
        if (!heap) abort();

        /* start with k=1, m=0 → (2)^2 + 1^2 = 5 */
        idx_t k = 1, m = 0;
        seq_t even_sq = (seq_t)(k * 2) * (seq_t)(k * 2);
        seq_t odd_sq  = (seq_t)(2 * m + 1) * (seq_t)(2 * m + 1);
        heap_push(even_sq + odd_sq, k, m);
        visited[(size_t)k][(size_t)m] = true;
        seq_len = 0;
    }

    /* generate terms until we have N of them */
    while (seq_len < (size_t)N) {
        node_t node = heap_pop();

        /* deduplicate equal sums (heap may contain duplicates). */
        if (seq_len == 0 || node.sum != seq_cache[seq_len - 1]) {
            seq_cache[seq_len++] = node.sum;
        }

        /* push the neighboring pairs (k+1,m) and (k,m+1) if unseen. */
        if (node.i + 1 < MAX_IDX && !visited[(size_t)(node.i + 1)][(size_t)node.j]) {
            idx_t k = node.i + 1, m = node.j;
            seq_t even_sq = (seq_t)(k * 2) * (seq_t)(k * 2);
            seq_t odd_sq  = (seq_t)(2 * m + 1) * (seq_t)(2 * m + 1);
            heap_push(even_sq + odd_sq, k, m);
            visited[(size_t)k][(size_t)m] = true;
        }
        if (node.j + 1 < MAX_IDX && !visited[(size_t)node.i][(size_t)(node.j + 1)]) {
            idx_t k = node.i, m = node.j + 1;
            seq_t even_sq = (seq_t)(k * 2) * (seq_t)(k * 2);
            seq_t odd_sq  = (seq_t)(2 * m + 1) * (seq_t)(2 * m + 1);
            heap_push(even_sq + odd_sq, k, m);
            visited[(size_t)k][(size_t)m] = true;
        }
    }
}

/* ---------- public function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but harmless */
    ensure_cache(n);
    return seq_cache[n - 1];
}
