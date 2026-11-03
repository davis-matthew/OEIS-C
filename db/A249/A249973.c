/*
 * A249973  Positive integers A when the positive roots of r^2 = Ar + B are
 *          listed in increasing order.
 * https://oeis.org/A249973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249973 – the A‑value of the pair (A,B) whose positive root of
   r^2 = Ar + B is the nth smallest.  The algorithm keeps a min‑heap
   of the next candidate (A,B) for each A and extracts the A of the
   nth pop.  Exact comparisons of r are performed with unsigned
   __int128 arithmetic, avoiding floating‑point.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* comparison of r = (A + sqrt(A^2+4B))/2.
   Returns <0 if pair1 < pair2, 0 if equal, >0 if pair1 > pair2.  */
static int cmp_pair(uint64_t A1, uint64_t B1,
                    uint64_t A2, uint64_t B2)
{
    /* values needed for comparison */
    unsigned __int128 D1 = (unsigned __int128)A1 * A1 + (unsigned __int128)4 * B1;
    unsigned __int128 D2 = (unsigned __int128)A2 * A2 + (unsigned __int128)4 * B2;

    /* the difference A2-A1 */
    int64_t diffA = (int64_t)A2 - (int64_t)A1;
    if (diffA < 0) {
        /* swap roles but negate the result */
        return -cmp_pair(A2, B2, A1, B1);
    }
    if (diffA == 0) {
        /* same A: compare D */
        if (D1 < D2) return -1;
        if (D1 > D2) return 1;
        /* equal r → tie by sqrt(A^2+B^2) */
        unsigned __int128 X1 = (unsigned __int128)A1 * A1 + (unsigned __int128)B1 * B1;
        unsigned __int128 X2 = (unsigned __int128)A2 * A2 + (unsigned __int128)B2 * B2;
        if (X1 < X2) return -1;
        if (X1 > X2) return 1;
        return 0;
    }
    /* diffA > 0 : compare r1 vs r2 using exact arithmetic */
    signed __int128 L = (signed __int128)D1 - (signed __int128)D2 -
                        (signed __int128)diffA * diffA;
    if (L < 0) return -1;  /* r1 < r2 */
    /* L >= 0 : check L^2 against 4*diffA^2*D2 */
    unsigned __int128 left  = (unsigned __int128)L * (unsigned __int128)L;
    unsigned __int128 right = (unsigned __int128)4 *
                              (unsigned __int128)diffA * (unsigned __int128)diffA *
                              D2;
    if (left < right) return -1;   /* r1 < r2 */
    if (left > right) return 1;    /* r1 > r2 */
    /* r1 == r2 → tie by sqrt(A^2+B^2) */
    unsigned __int128 X1 = (unsigned __int128)A1 * A1 + (unsigned __int128)B1 * B1;
    unsigned __int128 X2 = (unsigned __int128)A2 * A2 + (unsigned __int128)B2 * B2;
    if (X1 < X2) return -1;
    if (X1 > X2) return 1;
    return 0;
}

/* node representing a candidate pair (A,B) */
typedef struct {
    uint64_t A;
    uint64_t B;
} Node;

/*-----------------------------------------------------------------*/
/* Heap of pointers to Node (min‑heap by cmp_pair)                 */
typedef struct {
    Node **data;
    size_t size;
    size_t cap;
} Heap;

static Heap heap = {NULL, 0, 0};

static inline void heap_swap(Node **a, Node **b)
{
    Node *tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_sift_up(size_t i)
{
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (cmp_pair(heap.data[p]->A, heap.data[p]->B,
                     heap.data[i]->A, heap.data[i]->B) <= 0)
            break;
        heap_swap(&heap.data[p], &heap.data[i]);
        i = p;
    }
}

static void heap_sift_down(size_t i)
{
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap.size &&
            cmp_pair(heap.data[l]->A, heap.data[l]->B,
                     heap.data[smallest]->A, heap.data[smallest]->B) < 0)
            smallest = l;
        if (r < heap.size &&
            cmp_pair(heap.data[r]->A, heap.data[r]->B,
                     heap.data[smallest]->A, heap.data[smallest]->B) < 0)
            smallest = r;
        if (smallest == i) break;
        heap_swap(&heap.data[i], &heap.data[smallest]);
        i = smallest;
    }
}

static void heap_push(Node *node)
{
    if (heap.size >= heap.cap) {
        size_t newcap = heap.cap ? heap.cap * 2 : 16;
        Node **tmp = (Node **)realloc(heap.data, newcap * sizeof(Node *));
        if (!tmp) abort();
        heap.data = tmp;
        heap.cap = newcap;
    }
    heap.data[heap.size++] = node;
    heap_sift_up(heap.size - 1);
}

static Node *heap_pop(void)
{
    Node *node = heap.data[0];
    heap.data[0] = heap.data[--heap.size];
    heap_sift_down(0);
    return node;
}

/*-----------------------------------------------------------------*/
/* Cache of A-values in order of appearance                        */
typedef struct {
    seq_t *data;
    size_t size;
    size_t cap;
} Cache;

static Cache cache = {NULL, 0, 0};

static void cache_ensure(size_t need)
{
    if (need <= cache.cap) return;
    size_t newcap = cache.cap ? cache.cap * 2 : 64;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache.data, newcap);
    if (!tmp) abort();
    cache.data = tmp;
    cache.cap = newcap;
}

/* Initialise the heap with one candidate for each A.  20000 is
   sufficient for n up to 10 000. */
static void heap_init(void)
{
    const uint64_t MAX_A = 20000;
    heap.data = (Node **)calloc(MAX_A, sizeof(Node *));
    if (!heap.data) abort();
    heap.cap = MAX_A;
    for (uint64_t a = 1; a <= MAX_A; ++a) {
        Node *n = (Node *)malloc(sizeof(Node));
        if (!n) abort();
        n->A = a;
        n->B = 1;
        heap_push(n);
    }
    heap.size = MAX_A;
}

/*-----------------------------------------------------------------*/
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        heap_init();
        initialized = 1;
    }
    /* generate until we have at least n values in the cache */
    while (cache.size < (size_t)n) {
        Node *top = heap_pop();                     /* smallest (A,B) */
        cache_ensure(cache.size + 1);
        cache.data[cache.size++] = (seq_t)top->A;   /* store the A value */

        /* advance to next B for this A and push it back */
        top->B += 1;
        heap_push(top);
    }
    return cache.data[(size_t)n - 1];
}
