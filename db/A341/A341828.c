/*
 * A341828  Difference of consecutive odd squarefree semiprimes.
 * https://oeis.org/A341828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   build_primes_upto, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Difference of consecutive odd squarefree semiprimes:
   a(n) = p(n+1) – p(n), where p(k) is the k‑th odd squarefree semiprime.
   Offset 1 (a(1) is the first difference). */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/grow_seq.h"

static uint64_t *odd_primes = NULL;
static size_t odd_primes_cnt = 0;

/* Build odd primes up to a fixed limit. */
static void init_primes(void)
{
    if (odd_primes != NULL)
        return;

    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto(1000000, &primes, &primes_cnt);

    /* Count odd primes. */
    size_t oc = 0;
    for (size_t i = 0; i < primes_cnt; ++i)
        if (primes[i] & 1ULL)
            ++oc;

    odd_primes = (uint64_t *)malloc(oc * sizeof(uint64_t));
    size_t k = 0;
    for (size_t i = 0; i < primes_cnt; ++i)
        if (primes[i] & 1ULL)
            odd_primes[k++] = primes[i];

    odd_primes_cnt = oc;
    free(primes);
}

/* Binary heap of candidates for the next odd squarefree semiprime. */
typedef struct {
    seq_t  prod;   /* product p_i * p_j   */
    idx_t  i;      /* index of p_i (small prime)   */
    idx_t  j;      /* index of p_j (larger prime)   */
} Node;

static Node *heap_arr = NULL;
static size_t heap_size = 0;

/* Swap two heap nodes. */
static void swap_node(Node *a, Node *b)
{
    Node t = *a;
    *a = *b;
    *b = t;
}

/* Restore heap property by moving node at pos downwards. */
static void sift_down(size_t pos)
{
    while (1) {
        size_t left  = 2 * pos + 1;
        size_t right = left + 1;
        size_t smallest = pos;

        if (left < heap_size && heap_arr[left].prod < heap_arr[smallest].prod)
            smallest = left;
        if (right < heap_size && heap_arr[right].prod < heap_arr[smallest].prod)
            smallest = right;

        if (smallest == pos)
            break;
        swap_node(&heap_arr[pos], &heap_arr[smallest]);
        pos = smallest;
    }
}

/* Restore heap property by moving node at pos upwards. */
static void sift_up(size_t pos)
{
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (heap_arr[pos].prod < heap_arr[parent].prod) {
            swap_node(&heap_arr[pos], &heap_arr[parent]);
            pos = parent;
        } else
            break;
    }
}

/* Build the heap once. */
static void init_heap(void)
{
    if (heap_arr != NULL)
        return;

    init_primes();

    /* There are odd_primes_cnt odds, at least two of them. */
    size_t cap = odd_primes_cnt - 1;  /* one candidate per i */
    heap_arr = (Node *)malloc(cap * sizeof(Node));
    for (size_t i = 0; i < cap; ++i) {
        heap_arr[i].i   = i;
        heap_arr[i].j   = i + 1;
        heap_arr[i].prod = (seq_t)odd_primes[i] * odd_primes[i + 1];
    }
    heap_size = cap;
    for (size_t i = (cap >> 1); i-- > 0; )
        sift_down(i);
}

/* Pop the smallest node from the heap. */
static Node heap_pop(void)
{
    Node res = heap_arr[0];
    --heap_size;
    if (heap_size > 0) {
        heap_arr[0] = heap_arr[heap_size];
        sift_down(0);
    }
    return res;
}

/* Insert a node in the heap. */
static void heap_push(const Node *node)
{
    heap_arr[heap_size++] = *node;
    sift_up(heap_size - 1);
}

/* Storage for generated semiprimes and their successive differences. */
static seq_t *semip_arr  = NULL;
static size_t semip_cap  = 0;
static size_t semip_cnt  = 0;

static seq_t *diff_arr   = NULL;
static size_t diff_cap   = 0;
static size_t diff_cnt   = 0;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 1 */

    /* Ensure we have enough data. */
    idx_t need_semip = n + 1;          /* need nth+1 semiprime to compute diff(n) */
    while (semip_cnt < need_semip) {
        init_heap();                 /* heap ready for generation */

        /* Pop next smallest odd squarefree semiprime. */
        Node node   = heap_pop();
        seq_t val   = node.prod;
        if (semip_cnt > 0) {
            seq_t d = val - semip_arr[semip_cnt - 1];
            grow_seq(&diff_arr, &diff_cap, diff_cnt + 1);
            diff_arr[diff_cnt++] = d;
        }

        grow_seq(&semip_arr, &semip_cap, semip_cnt + 1);
        semip_arr[semip_cnt++] = val;

        /* Advance to the next candidate for the same small prime i. */
        if (node.j + 1 < odd_primes_cnt) {
            node.j++;
            node.prod = (seq_t)odd_primes[node.i] * odd_primes[node.j];
            heap_push(&node);
        }
    }

    return diff_arr[n - 1];
}
