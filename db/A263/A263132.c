/*
 * A263132  Positive values of m such that binomial(4*m - 1, m) is odd.
 * https://oeis.org/A263132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th smallest number of the form 4^i * ceil(2^k/3) (i
 *            ≥ 0, k ≥ 1), sorted in ascending order. This matches OEIS
 *            A263132
 * Range:     u8 n <= 16, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* internal utilities ****************************************************/

static inline seq_t pow2_shift(idx_t exp) {
    /* compute 2^exp, with exp < width of seq_t */
    return ((seq_t)1) << (exp);
}
static inline seq_t ceil_pow2_div3(idx_t exp) {
    /* ceil(2^exp / 3) = (2^exp + 2) / 3 */
    return (pow2_shift(exp) + (seq_t)2) / (seq_t)3;
}

/* node in the min‑heap                                           */
typedef struct {
    seq_t val;   /* next value from this chain */
    idx_t n;     /* 4^n factor index  (power of 4) */
    idx_t k;     /* next k to use: ceil(2^k/3)                       */
} node_t;

/* maximum number of chains we need to store in the heap.
   For a seq_t of W bits, 4^n fits only while 2*n < W,
   so we allow n = 0 … (W-1)/2. */
#define N_MAX ((sizeof(seq_t)*8 - 1)/2)

/* static state across calls *********************************************/

static node_t heap[N_MAX + 1];
static size_t heap_size = 0;

/* cache of already generated terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* heap helpers -------------------------------------------------------*/

static void heap_swap(size_t i, size_t j) {
    node_t tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heapify_down(size_t i) {
    while (1) {
        size_t left = i * 2 + 1;
        size_t right = left + 1;
        size_t smallest = i;
        if (left < heap_size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val)
            smallest = right;
        if (smallest == i) break;
        heap_swap(i, smallest);
        i = smallest;
    }
}

static void heapify_up(size_t i) {
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        heap_swap(i, parent);
        i = parent;
    }
}

/* initialization ------------------------------------------------------*/

static void init_once(void) {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* build initial heap: one chain for each n  (k = 1  →  ceil(2^1/3) = 1) */
    for (idx_t i = 0; i <= N_MAX; ++i) {
        heap[i].n = i;
        heap[i].k = 1;
        heap[i].val = pow2_shift(2 * i);          /* 4^i * 1 = 4^i */
    }
    heap_size = N_MAX + 1;
    for (size_t i = (heap_size >> 1); i-- > 0; ) heapify_down(i);

    /* prepare cache */
    cache_cap = 128;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_size = 0;
}

/* main routine -------------------------------------------------------*/

seq_t a(idx_t n) {
    init_once();

    if (n == 0) return 0;          /* the sequence is defined for n ≥ 1 */

    /* grow cache if necessary */
    while (cache_size < n) {
        /* smallest element in heap */
        node_t cur = heap[0];

        /* store the value in the cache */
        if (cache_size >= cache_cap) {
            cache_cap = (cache_cap == 0) ? 128 : cache_cap << 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_size++] = cur.val;

        /* advance this chain: increase k */
        idx_t next_k = cur.k + 1;
        /* new value: 4^n * ceil(2^next_k/3) */
        seq_t t = ceil_pow2_div3(next_k);
        seq_t new_val = pow2_shift(2 * cur.n) * t;
        /* check for overflow: if multiplication wrapped, stop this chain */
        if (new_val < pow2_shift(2 * cur.n)) {
            heap[0].val = (seq_t)-1; /* put a very large sentinel */
            heap[0].k = next_k;
        } else {
            heap[0].val = new_val;
            heap[0].k   = next_k;
        }
        heapify_down(0);
    }

    return cache[n - 1];
}
