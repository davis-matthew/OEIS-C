/*
 * A338899  Concatenated sequence of prime indices of squarefree semiprimes
 *          (A006881).
 * https://oeis.org/A338899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear, tabf, changed
 * Formula:   a(n) is the concatenated, in order of squarefree semiprimes, of
 *            the prime indices of each semiprime. For each squarefree
 *            semiprime p*q with p<q, let i=π(p), j=π(q) (π = prime index,
 *            2→1, 3→2, 5→3, …). The sequence lists (i,j) for every semiprime
 *            in increasing numeric order, flattening the pairs s...
 * Range:     u8 n <= 576, u16 n <= 576, u32 n <= 576, u64 n <= 576,
 *            u128 n <= 576, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t idx_t; /* as supplied by build */

static uint64_t *primes = NULL;   /* list of primes */
static size_t primes_cnt = 0;     /* number of primes */

typedef struct {
    uint64_t prod; /* product p_i * p_j, i < j  */
    size_t i;      /* index of the first prime (0‑based) */
    size_t j;      /* index of the second prime (0‑based) */
} HeapNode;

static HeapNode *heap = NULL;     /* binary heap of nodes */
static size_t heap_sz = 0;        /* current heap size */

static seq_t *cache = NULL;       /* concatenated indices */
static size_t cache_len = 0;      /* number of stored terms */
static size_t cache_cap = 0;      /* cache capacity */

static size_t semis_done = 0;     /* number of semiprimes already generated */

/* ---------- helpers for the binary heap ---------- */
static void heap_swap(size_t a, size_t b)
{
    HeapNode tmp = heap[a];
    heap[a] = heap[b];
    heap[b] = tmp;
}

static void heap_sift_up(size_t idx)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[parent].prod <= heap[idx].prod) break;
        heap_swap(parent, idx);
        idx = parent;
    }
}

static void heap_sift_down(size_t idx)
{
    while (1) {
        size_t left = (idx << 1) + 1;
        if (left >= heap_sz) break;
        size_t right = left + 1;
        size_t child = left;
        if (right < heap_sz && heap[right].prod < heap[left].prod)
            child = right;
        if (heap[idx].prod <= heap[child].prod) break;
        heap_swap(idx, child);
        idx = child;
    }
}

static void heap_init(void)
{
    /* build a heap that contains the first element of each row
       (p_i * p_{i+1}) for all i < primes_cnt-1                     */
    heap_sz = primes_cnt - 1;
    if (!heap_sz) return;
    heap = (HeapNode *)malloc(heap_sz * sizeof(HeapNode));
    for (size_t i = 0; i < heap_sz; ++i) {
        heap[i].i  = i;
        heap[i].j  = i + 1;
        heap[i].prod = (uint64_t)primes[i] * primes[i+1];
    }
    /* build heap in linear time */
    for (size_t i = (heap_sz >> 1); i-- > 0; ) heap_sift_down(i);
}

static void heap_push_next(HeapNode node)
{
    /* node already contains correct prod, i, j; push into heap */
    heap[heap_sz++] = node;
    heap_sift_up(heap_sz - 1);
}

/* pop the smallest element and return it    */
static HeapNode heap_pop(void)
{
    HeapNode min = heap[0];
    heap[0] = heap[--heap_sz];
    if (heap_sz) heap_sift_down(0);
    return min;
}

/* ---------- sieve to generate primes up to limit */ 
static void generate_primes(size_t limit)
{
    uint8_t *is_composite = (uint8_t *)calloc(limit + 1, 1);
    if (!is_composite) exit(1); /* out of memory */

    for (size_t p = 2; p * p <= limit; ++p) {
        if (!is_composite[p]) {
            for (size_t k = p * p; k <= limit; k += p)
                is_composite[k] = 1;
        }
    }
    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i) if (!is_composite[i]) ++cnt;

    primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) primes[idx++] = i;

    primes_cnt = cnt;
    free(is_composite);
}

/* ---------- dynamic cache for concatenated indices ---------- */
static void cache_init(void)
{
    cache_cap = 256;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache) exit(1);
    cache_len = 0;
}

/* enlarge cache if needed */
static void cache_grow(void)
{
    if (cache_len < cache_cap) return;
    cache_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;
}

/* append an index to the cache */
static void cache_append(seq_t val)
{
    cache_grow();
    cache[cache_len++] = val;
}

/* generate next semiprime and store its two indices in cache */
static void generate_next_semiprime(void)
{
    HeapNode node = heap_pop();

    /* output its indices (1‑based) */
    cache_append((seq_t)(node.i + 1));
    cache_append((seq_t)(node.j + 1));
    semis_done++;

    /* push next element from the same row, if any */
    if (node.j + 1 < primes_cnt) {
        HeapNode nxt;
        nxt.i = node.i;
        nxt.j = node.j + 1;
        nxt.prod = (uint64_t)primes[nxt.i] * primes[nxt.j];
        heap_push_next(nxt);
    }
}

/* ------------------------------------------ main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* behaviour below offset irrelevant */

    /* initialize once */
    enum { INIT = 1 };
    static int initialized = 0;
    if (!initialized) {
        /* choose a limit that is safe for n up to about 10 000 */
        generate_primes(1000000);   /* 1 000 000 gives ~78 500 primes  */
        heap_init();
        cache_init();
        initialized = INIT;
    }

    size_t needed_semis = (size_t)((n + 1) >> 1); /* ceil(n/2) */

    while (semis_done < needed_semis) {
        generate_next_semiprime();
    }

    return cache[n - 1];
}
