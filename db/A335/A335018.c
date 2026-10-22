/*
 * A335018  Number of triples (d1,d2,d3) where each element is a divisor of m
 *          and d1 + d2 + d3 <= m where m is least odd integer of each prime
 *          signature.
 * https://oeis.org/A335018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (tau(A147516(n)) - 1)^3
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any n >= 1; stores minimal odd integers by prime signature
   using a min‑heap and memoises results. */
/* Requires: seq_t and idx_t types defined by the build system. */
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>          /* for size_t */

/* ------------------------------------------------------------------ */
/*  Small array of the first 32 odd primes.                           */
/* ------------------------------------------------------------------ */
static const uint64_t odd_primes[32] = {
    3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL,
    29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL,
    61ULL, 67ULL, 71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL,
    101ULL,103ULL,107ULL,109ULL,113ULL,127ULL
};

/* ------------------------------------------------------------------ */
/*  Node used in the min‑heap: minimal odd integer of a prime         */
/*  signature and its exponent vector (non‑increasing).                */
/* ------------------------------------------------------------------ */
typedef struct Node {
    seq_t          val;          /* minimal odd integer for this signature */
    uint8_t        exps[32];     /* exponents of primes: non‑increasing */
    size_t         len;          /* number of primes used                 */
} Node;

/* ------------------------------------------------------------------ */
/*  Simple binary min‑heap of Node* values.                           */
/* ------------------------------------------------------------------ */
static Node **heap      = NULL;   /* array of pointers to Node */
static size_t heap_size = 0;      /* current number of elements */
static size_t heap_cap  = 0;      /* allocated capacity */

/* Ensure the heap has at least 'need' slots. */
static void heap_reserve(size_t need)
{
    if (need > heap_cap) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        heap = (Node **)realloc(heap, new_cap * sizeof(Node *));
        heap_cap = new_cap;
    }
}

/* Compare two nodes by their value.  Return <0 if a<b, 0 if a==b, >0 if a>b. */
static int heap_cmp(Node *a, Node *b)
{
    if (a->val < b->val) return -1;
    if (a->val > b->val) return 1;
    return 0;
}

/* Push a node onto the heap. */
static void heap_push(Node *n)
{
    heap_reserve(heap_size + 1);
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap_cmp(heap[p], n) <= 0) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = n;
}

/* Pop the smallest node from the heap. */
static Node *heap_pop(void)
{
    if (heap_size == 0) return NULL;
    Node *top = heap[0];
    heap[0] = heap[--heap_size];
    size_t i = 0, end = heap_size;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= end) break;
        size_t r = l + 1;
        size_t best = l;
        if (r < end && heap_cmp(heap[r], heap[l]) < 0) best = r;
        if (heap_cmp(heap[best], heap[i]) >= 0) break;
        Node *tmp = heap[i];
        heap[i] = heap[best];
        heap[best] = tmp;
        i = best;
    }
    return top;
}

/* ------------------------------------------------------------------ */
/*  Cache for computed a(n) values.                                    */
/* ------------------------------------------------------------------ */
static seq_t *a_cache = NULL;          /* 1‑based index; 0 unused */
static idx_t a_cache_size = 0;         /* highest index stored */
static size_t a_cache_cap = 0;         /* allocated capacity */

/* Ensure the cache can hold at least 'need' indices (1‑based). */
static void a_cache_reserve(size_t need)
{
    if (need > a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache_cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/*  Generate a(n) values up to 'target' (target >= 1).                 */
/* ------------------------------------------------------------------ */
static void generate_terms(idx_t target)
{
    /* Initialise heap and cache on first call. */
    if (heap == NULL) {
        heap_cap = 64;
        heap = (Node **)malloc(heap_cap * sizeof(Node *));
        heap_size = 0;
        Node *root = (Node *)malloc(sizeof(Node));
        root->val = (seq_t)1;
        root->len = 0;
        heap_push(root);
    }

    a_cache_reserve(target + 1);   /* space for indices 1..target inclusive */

    /* Extend cache only if needed. */
    if (a_cache_size < target + 1) {
        for (idx_t i = a_cache_size + 1; i <= target; ++i) {
            Node *cur = heap_pop();   /* lowest value not yet processed */

            /* tau(m) = product of (exponent + 1) */
            seq_t tau = (seq_t)1;
            for (size_t j = 0; j < cur->len; ++j)
                tau = tau * (seq_t)(cur->exps[j] + 1);

            seq_t base = tau - (seq_t)1;   /* tau - 1 */
            seq_t val  = base * base * base;   /* (tau-1)^3 */

            a_cache_reserve(i + 1);  /* ensure capacity for this index */
            a_cache[i] = val;
            ++a_cache_size;

            /* Generate children: increment last exponent or add new prime. */
            if (cur->len == 0) {   /* start from the first odd integer (3) */
                Node *child = (Node *)malloc(sizeof(Node));
                child->len = 1;
                child->exps[0] = 1;
                child->val = cur->val * (seq_t)odd_primes[0];
                heap_push(child);
            } else {
                /* Child 1: increase last exponent if still non‑increasing. */
                uint8_t last = cur->exps[cur->len - 1];
                if (cur->len == 1 || (last + 1) <= cur->exps[cur->len - 2]) {
                    Node *child = (Node *)malloc(sizeof(Node));
                    child->len = cur->len;
                    memcpy(child->exps, cur->exps, cur->len);
                    child->exps[cur->len - 1] = last + 1;
                    child->val = cur->val * (seq_t)odd_primes[cur->len - 1];
                    heap_push(child);
                }
                /* Child 2: append a new prime with exponent 1. */
                Node *child = (Node *)malloc(sizeof(Node));
                child->len = cur->len + 1;
                memcpy(child->exps, cur->exps, cur->len);
                child->exps[cur->len] = 1;
                child->val = cur->val * (seq_t)odd_primes[cur->len];
                heap_push(child);
            }

            free(cur);   /* current node processed */
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Public API required by the problem statement.                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but return 0 for safety */
    generate_terms(n);
    return a_cache[n];
}
