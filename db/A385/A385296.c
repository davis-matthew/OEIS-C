/*
 * A385296  Numbers whose digits all belong to the same residue class mod 7.
 * https://oeis.org/A385296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 126, u32 n <= 1993, u64 n <= 8742,
 *            u128 n <= 8742, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose digits all belong to the same residue
 * class modulo 7.  The numbers are generated in increasing order
 * by a min‑heap that expands each number by appending all allowed
 * digits from its residue class.  This yields the OEIS sequence
 * A385296. */

#include <stdlib.h>

/* ----- digits per residue class ------------------------------------ */
static const uint8_t digits0[2] = {0, 7};
static const uint8_t digits1[2] = {1, 8};
static const uint8_t digits2[2] = {2, 9};
static const uint8_t digits3[1] = {3};
static const uint8_t digits4[1] = {4};
static const uint8_t digits5[1] = {5};
static const uint8_t digits6[1] = {6};

static const uint8_t *digits_arr[7] = {
    digits0, digits1, digits2, digits3,
    digits4, digits5, digits6
};

static const size_t digits_cnt[7] = {2, 2, 2, 1, 1, 1, 1};

/* ----- heap node --------------------------------------------------- */
typedef struct {
    seq_t val;   /* decimal value */
    int   r;     /* residue class (0…6) */
} Node;

/* ----- global state (cache + heap) --------------------------------- */
static seq_t  *cache     = NULL;
static size_t  cache_size = 0;
static size_t  cache_cap  = 0;

static Node  *heap      = NULL;
static size_t  heap_size = 0;
static size_t  heap_cap  = 0;

/* ----- utility: dynamic array for cache ----------------------------- */
static void cache_push(seq_t v)
{
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = v;
}

/* ----- heap operations --------------------------------------------- */
static void heap_push(Node n)
{
    if (heap_size == heap_cap) {
        size_t new_cap = (heap_cap == 0) ? 64 : heap_cap * 2;
        Node *tmp = (Node *)realloc(heap, new_cap * sizeof(Node));
        if (!tmp) exit(1);
        heap = tmp;
        heap_cap = new_cap;
    }
    /* place at the end */
    size_t i = heap_size++;
    heap[i] = n;
    /* bubble up */
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[parent].val <= heap[i].val) break;
        Node tmp = heap[parent];
        heap[parent] = heap[i];
        heap[i] = tmp;
        i = parent;
    }
}

static Node heap_pop(void)
{
    Node top = heap[0];
    heap[0] = heap[--heap_size];
    /* bubble down */
    size_t i = 0;
    while (1) {
        size_t left = (i << 1) + 1;
        if (left >= heap_size) break;
        size_t right = left + 1;
        size_t min_child = (right < heap_size && heap[right].val < heap[left].val) ? right : left;
        if (heap[i].val <= heap[min_child].val) break;
        Node tmp = heap[i];
        heap[i] = heap[min_child];
        heap[min_child] = tmp;
        i = min_child;
    }
    return top;
}

/* ----- initialization of heap with single‑digit numbers -------------- */
static void heap_initialize(void)
{
    for (int r = 0; r <= 6; ++r) {
        for (size_t i = 0; i < digits_cnt[r]; ++i) {
            Node n = { digits_arr[r][i], r };
            heap_push(n);
        }
    }
}

/* ----- main sequence function ------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour for 0 is irrelevant */
        return 0;

    /* initialize if first call */
    if (cache_size == 0 && heap_size == 0)
        heap_initialize();

    /* produce terms until we have at least n */
    while (cache_size < (size_t)n) {
        Node cur = heap_pop();
        cache_push(cur.val);

        /* generate extensions of this number */
        if (cur.val != 0) {
            for (size_t i = 0; i < digits_cnt[cur.r]; ++i) {
                seq_t child = cur.val * 10 + (seq_t)digits_arr[cur.r][i];
                Node nxt = { child, cur.r };
                heap_push(nxt);
            }
        }
    }

    return cache[n - 1];
}
