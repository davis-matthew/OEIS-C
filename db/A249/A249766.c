/*
 * A249766  Numbers k = concat(s,t) such that s and t divide k.
 * https://oeis.org/A249766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A249766
   a(n) = the n-th smallest positive integer k that can be written as
          k = concat(s,t) (t may have leading zeros), with s | k and t | k.
   (k = s * 10^d + t, for some integer d ≥ 1, 0 ≤ t < 10^d,
    s, t, k > 0). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* The following types are defined by the build system */
extern unsigned long long isqrt_idx(unsigned long long);

/* Helper functions (may be provided by the utilities).  We only use
   those that are guaranteed to exist.  In this implementation we
   avoid external utilities except for memory allocation and
   comparison of seq_t.  For arbitrary‑precision builds seq_t
   supports the usual arithmetic and relational operators. */

static const size_t MAX_TERMS = 100000;      /* should cover n up to 10000 */
static const int D_MAX      = 9;             /* 10^9 < 2^63, safe for 64‑bit */

static seq_t *seq_cache = NULL;             /* sorted, unique k's */
static size_t seq_len = 0;                  /* number of generated terms */

/* The generator node kept in the min‑heap */
struct Node {
    seq_t k;      /* next value of this generator */
    seq_t base;   /* 10^d + q (step size) */
    seq_t s;      /* current multiplier (starts at 1) */
    seq_t s_max;  /* maximum allowed multiplier */
};

/* ------------------------------------------- */
/* Min‑heap implementation (binary heap) */
static struct Node *heap = NULL;
static size_t heap_size = 0;
static size_t heap_cap  = 0;

static void heap_init(size_t cap)
{
    heap_cap = cap;
    heap = (struct Node *)malloc(cap * sizeof(struct Node));
    heap_size = 0;
}

static inline void heap_swap(size_t i, size_t j)
{
    struct Node tmp = heap[i];
    heap[i] = heap[j];
    heap[j] = tmp;
}

static void heap_push(struct Node n)
{
    if (heap_size == heap_cap) {
        heap_cap = heap_cap == 0 ? 128 : heap_cap << 1;
        heap = (struct Node *)realloc(heap, heap_cap * sizeof(struct Node));
    }
    size_t idx = heap_size++;
    heap[idx] = n;
    while (idx > 0) {
        size_t p = (idx - 1) >> 1;
        if (heap[idx].k >= heap[p].k) break;
        heap_swap(idx, p);
        idx = p;
    }
}

static struct Node heap_pop(void)
{
    struct Node top = heap[0];
    --heap_size;
    if (heap_size > 0) {
        heap[0] = heap[heap_size];
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            size_t r = l + 1;
            size_t smallest = i;
            if (l < heap_size && heap[l].k < heap[smallest].k) smallest = l;
            if (r < heap_size && heap[r].k < heap[smallest].k) smallest = r;
            if (smallest == i) break;
            heap_swap(i, smallest);
            i = smallest;
        }
    }
    return top;
}
/* ------------------------------------------- */

/* Build the sequence once */
static void build_sequence(void)
{
    if (seq_cache != NULL) return;  /* already built */

    /* Precompute powers of 10, 2, and 5 */
    seq_t pow10[D_MAX + 1];
    seq_t pow2 [D_MAX + 1];
    seq_t pow5 [D_MAX + 1];
    pow10[0] = 1;  pow2[0] = 1;  pow5[0] = 1;
    for (int i = 1; i <= D_MAX; ++i) {
        pow10[i] = pow10[i-1] * (seq_t)10;
        pow2 [i] = pow2 [i-1] * (seq_t)2;
        pow5 [i] = pow5 [i-1] * (seq_t)5;
    }

    /* Create a list of initial generators (one per (d,q) pair) */
    struct Node *generators = NULL;
    size_t gen_cap = 0;
    size_t gen_count = 0;
    /* Maximum number of generators is sum_{d=1}^{D_MAX}(d+1)^2 <= 400 */
    gen_cap = 500;   /* safe upper bound */
    generators = (struct Node *)malloc(gen_cap * sizeof(struct Node));

    for (int d = 1; d <= D_MAX; ++d) {
        seq_t ten_d = pow10[d];
        for (int i = 0; i <= d; ++i) {
            seq_t p2 = pow2[i];
            for (int j = 0; j <= d; ++j) {
                seq_t p5 = pow5[j];
                seq_t q = p2 * p5;              /* divisor of 10^d, >=1 */
                if (q == 0) continue;           /* safety */

                /* s must satisfy s*q < 10^d  =>  s ≤ (10^d-1)/q */
                seq_t s_max = (ten_d - (seq_t)1) / q;
                if (s_max < (seq_t)1) continue; /* no valid s */

                struct Node n;
                n.base   = ten_d + q;          /* step size */
                n.s_max  = s_max;
                n.s      = (seq_t)1;
                n.k      = n.base * n.s;       /* first value: base*1 */

                if (gen_count == gen_cap) {
                    gen_cap <<= 1;
                    generators = (struct Node *)realloc(generators, gen_cap * sizeof(struct Node));
                }
                generators[gen_count++] = n;
            }
        }
    }

    /* Prepare heap and output cache */
    heap_init(gen_count);
    seq_cache = (seq_t *)malloc(MAX_TERMS * sizeof(seq_t));
    seq_len = 0;

    /* Push all initial generators into the heap */
    for (size_t i = 0; i < gen_count; ++i) heap_push(generators[i]);

    /* Free generators array (we no longer need initial copies) */
    free(generators);

    /* Generate terms until we have enough or the heap is exhausted */
    while (seq_len < MAX_TERMS && heap_size > 0) {
        struct Node cur = heap_pop();

        /* Deduplicate: skip if same as previous term */
        if (seq_len == 0 || cur.k != seq_cache[seq_len-1]) {
            seq_cache[seq_len++] = cur.k;
        }

        /* Advance generator if more values available */
        if (cur.s < cur.s_max) {
            ++cur.s;
            cur.k += cur.base; /* next value: base * s */
            heap_push(cur);
        }
    }

    /* If we didn't reach MAX_TERMS, shrink the cache */
    if (seq_len < MAX_TERMS) {
        seq_cache = (seq_t *)realloc(seq_cache, seq_len * sizeof(seq_t));
    }
}
/* ------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n=0 */
    build_sequence();                        /* ensure cache is ready */
    if (n <= (idx_t)seq_len) {
        return seq_cache[n-1];
    }
    /* If requested index exceeds precomputed size, return 0 */
    return (seq_t)0;
}
