/*
 * A319184  Numbers that are sums of consecutive pentagonal numbers.
 * https://oeis.org/A319184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integers that are sums of consecutive pentagonal
 *            numbers; computed via merging of partial sums
 * Range:     u8 n <= 13, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
#include <new>
#endif

/* Minimum heap node: value and the indices of the running sum */
struct Node {
    seq_t val;
    idx_t i;   /* starting index of the sequence */
    idx_t j;   /* ending index of the sequence (current) */
};

/* Helper: push a node onto a min‑heap */
static inline void heap_push(struct Node *data, size_t *size, size_t cap, struct Node n)
{
    size_t idx = *size;
    (*size)++;
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (data[parent].val <= n.val)
            break;
        data[idx] = data[parent];
        idx = parent;
    }
    data[idx] = n;
}

/* Helper: pop the smallest node from a min‑heap */
static inline struct Node heap_pop(struct Node *data, size_t *size)
{
    struct Node res = data[0];
    struct Node last = data[--(*size)];
    size_t idx = 0;
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= *size)
            break;
        size_t right = left + 1;
        size_t child = (right < *size && data[right].val < data[left].val) ? right : left;
        if (data[child].val >= last.val)
            break;
        data[idx] = data[child];
        idx = child;
    }
    data[idx] = last;
    return res;
}

/* Compute the k‑th pentagonal number   P(k) = k(3k−1)/2   with k >= 1 */
static inline seq_t pentagonal(idx_t k)
{
    seq_t kk = (seq_t)k;
    return kk * ((seq_t)(3 * kk) - 1) / 2;
}

/* Static cache of the sequence */
static seq_t *g_cache = NULL;
static idx_t   g_cache_len = 0;
static idx_t   g_cache_cap = 0;

/* Allocate or grow the cache to hold at least n terms */
static void ensure_cache(idx_t n)
{
    if (n <= g_cache_cap)
        return;
    seq_t *new_cache = NULL;
#ifdef __cplusplus
    new_cache = new seq_t[n];
#else
    new_cache = (seq_t*)realloc(g_cache, n * sizeof(seq_t));
#endif
    if (!new_cache)    /* out of memory – abort silently */
        return;
    g_cache = new_cache;
    g_cache_cap = n;
}

/* Ensure the cache contains at least n terms. The algorithm merges
   the sequences of partial sums of pentagonal numbers. */
static void compute_cache(idx_t n)
{
    if (g_cache_len >= n)
        return;

    /* Allocate enough storage for n terms */
    ensure_cache(n);
    if (!g_cache)
        return;    /* allocation failed – give up */

    /* Reset the cache and insert the first term a(1) = 0 */
    g_cache_len = 0;
    g_cache[0] = 0;
    g_cache_len = 1;

    /* Allocate a heap large enough for n starting sequences */
    size_t heap_cap = n;
    struct Node *heap = NULL;
#ifdef __cplusplus
    heap = new struct Node[heap_cap];
#else
    heap = (struct Node*)malloc(heap_cap * sizeof(struct Node));
#endif
    size_t hsize = 0;

    /* Each sequence starts with a single pentagonal number P(i) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = pentagonal(i);
        heap_push(heap, &hsize, heap_cap, (struct Node){val, i, i});
    }

    seq_t last_val;
    bool first_output = true;

    /* Generate terms until we have n values */
    while (g_cache_len < n) {
        struct Node node = heap_pop(heap, &hsize);
        if (first_output || node.val != last_val) {
            g_cache[g_cache_len++] = node.val;
            last_val = node.val;
            first_output = false;
        }
        /* Advance the current sequence: add the next pentagonal number */
        seq_t next_val = node.val + pentagonal(node.j + 1);
        heap_push(heap,
                  &hsize,
                  heap_cap,
                  (struct Node){next_val, node.i, node.j + 1});
    }

    /* Free the heap */
#ifdef __cplusplus
    delete[] heap;
#else
    free(heap);
#endif
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    /* OEIS offset: a(1) = 0 */
    if (n == 0)
        return 0;               /* undefined term – return 0 */
    compute_cache(n);
    return g_cache[n - 1];
}
