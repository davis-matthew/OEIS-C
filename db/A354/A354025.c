/*
 * A354025  Terms m of A351467 such that A005245(m) == 2 (mod 3).
 * https://oeis.org/A354025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A354025: a(n) is the nth integer m such that A005245(m) ≡ 2 (mod 3). 
   It consists of all integers of the form 
        2^p * 3^r          (p ∈ {1,4,7,10}, r ≥ 0)
      | 4(3^r + 1) * 3^s   (r > 0 , s ≥ 0)
      | 2(2·3^r + 1) * 3^s (r ≥ 0 , s ≥ 0)
      | (4·3^r + 1) * 3^s (r ≥ 0 , s ≥ 0).
   The implementation builds all base values up to a large fixed limit,
   then merges the infinite sequences base·3^s via a min‑heap.
   Duplicate values are discarded.  The algorithm runs in O(n log k)
   where k is the number of base generation patterns (k = 7 in this case).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- Node and heap ----------------------------------------------------- */
typedef struct {
    seq_t val;   /* current value = base · 3^exponent */
    seq_t base;  /* invariant base factor                 */
} Node;

static Node *heap = NULL;
static size_t heap_len = 0;
static size_t heap_cap = 0;

/* ---- cache for produced terms ---------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ---- initialization flag --------------------------------------------- */
static bool initialized = false;

/* ---- heap helpers ----------------------------------------------------- */
static void heap_swap(size_t i, size_t j)
{
    Node tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(i, p);
        i = p;
    }
}

static void sift_down(size_t i)
{
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_len && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_len && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        heap_swap(i, smallest);
        i = smallest;
    }
}

static void heap_push(Node n)
{
    if (heap_len == heap_cap) {
        size_t new_cap = heap_cap ? heap_cap << 1 : 64;
        Node *tmp = (Node *)realloc(heap, new_cap * sizeof(Node));
        if (!tmp) abort();
        heap = tmp;
        heap_cap = new_cap;
    }
    heap[heap_len++] = n;
    sift_up(heap_len - 1);
}

static Node heap_pop(void)
{
    Node min = heap[0];
    heap[0] = heap[--heap_len];
    if (heap_len > 0) sift_down(0);
    return min;
}

/* ---- cache helpers --------------------------------------------------- */
static void cache_ensure(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ---- base generation ------------------------------------------------- */
static void init(void)
{
    if (initialized) return;

    /* allocate heap and cache */
    heap_cap = 64;
    heap = (Node *)calloc(heap_cap, sizeof(Node));
    cache_cap = 64;
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));

    /* Limit for base generation – large enough to cover n up to many thousands */
    const seq_t BASE_LIMIT = (seq_t)1000000000000ULL;  /* 1e12 */

    /* 1. powers 2^p with p in {1,4,7,10} */
    const int p_vals[] = {1,4,7,10};
    for (size_t pi = 0; pi < sizeof(p_vals)/sizeof(p_vals[0]); ++pi) {
        int p = p_vals[pi];
        seq_t pow2 = 1;
        for (int i = 0; i < p; ++i) pow2 *= 2;
        seq_t pow3 = 1;
        while (1) {
            seq_t base = pow2 * pow3;
            if (base > BASE_LIMIT) break;
            heap_push((Node){base, base});
            pow3 *= 3;
        }
    }

    /* 2. 4(3^r + 1) for r > 0 */
    seq_t pow3 = 3;
    for (int r = 1; ; ++r) {   /* r starts at 1 */
        seq_t base = 4 * (pow3 + 1);
        if (base > BASE_LIMIT) break;
        heap_push((Node){base, base});
        pow3 *= 3;
    }

    /* 3. 2(2·3^r + 1) for r ≥ 0 */
    pow3 = 1;
    for (int r = 0; ; ++r) {
        seq_t base = 2 * (2 * pow3 + 1);
        if (base > BASE_LIMIT) break;
        heap_push((Node){base, base});
        pow3 *= 3;
    }

    /* 4. (4·3^r + 1) for r ≥ 0 */
    pow3 = 1;
    for (int r = 0; ; ++r) {
        seq_t base = 4 * pow3 + 1;
        if (base > BASE_LIMIT) break;
        heap_push((Node){base, base});
        pow3 *= 3;
    }

    initialized = true;
}

/* ---- public function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset 1 */

    init();

    while (cache_len < n) {
        Node cur = heap_pop();

        /* discard duplicates */
        if (cache_len == 0 || cur.val != cache[cache_len - 1]) {
            cache_ensure(cache_len + 1);
            cache[cache_len++] = cur.val;
        }

        /* enqueue next multiple by 3 */
        Node next = cur;
        next.val = cur.val * 3;
        heap_push(next);
    }

    return cache[n - 1];
}
