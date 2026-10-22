/*
 * A340771  Numbers which are the sum of some number of consecutive prime
 *          squares.
 * https://oeis.org/A340771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1695, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   would_overflow_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A340771: numbers that are the sum of some number of consecutive prime squares. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/would_overflow_sum.h"

/* helper: generate all primes up to limit */
static void generate_primes(idx_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    bool *is_composite = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_composite) { *primes_out = NULL; *cnt_out = 0; return; }
    memset(is_composite, 0, (limit + 1) * sizeof(bool));

    for (idx_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++cnt;

    uint32_t *primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) primes[pos++] = (uint32_t)i;

    free(is_composite);
    *primes_out = primes;
    *cnt_out    = cnt;
}

/* node used in the min‑heap */
typedef struct
{
    seq_t sum;    /* current sum of consecutive prime squares */
    idx_t next;   /* index of the next prime to add for this block */
} Node;

/* min‑heap helpers */
static inline void heap_swap(Node *a, Node *b)
{
    Node tmp = *a; *a = *b; *b = tmp;
}

static void sift_up(Node *heap, size_t idx)
{
    while (idx > 0)
    {
        size_t parent = (idx - 1) / 2;
        if (heap[parent].sum <= heap[idx].sum) break;
        heap_swap(&heap[parent], &heap[idx]);
        idx = parent;
    }
}

static void sift_down(Node *heap, size_t size, size_t idx)
{
    while (1)
    {
        size_t left  = 2 * idx + 1;
        size_t right = 2 * idx + 2;
        size_t smallest = idx;

        if (left  < size && heap[left].sum  < heap[smallest].sum) smallest = left;
        if (right < size && heap[right].sum < heap[smallest].sum) smallest = right;

        if (smallest == idx) break;
        heap_swap(&heap[smallest], &heap[idx]);
        idx = smallest;
    }
}

static void heap_push(Node *heap, size_t *size, Node node)
{
    heap[*size] = node;
    sift_up(heap, *size);
    ++(*size);
}

static Node heap_pop(Node *heap, size_t *size)
{
    Node top = heap[0];
    --(*size);
    heap[0] = heap[*size];
    sift_down(heap, *size, 0);
    return top;
}

/* helper to detect overflow of seq_t addition (works for native unsigned types up to 64 bit) */
#if defined(__SIZEOF_INT128__)
#  define HAVE_UINT128 1
#else
#  define HAVE_UINT128 0
#endif

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for index 0 */

    /* 1. Prepare primes – a limit that guarantees at least n distinct sums.
       For n ≈ 10 000 a few thousand primes are more than enough. */
    const idx_t PRIME_LIMIT = 200000;   /* 200 000 gives ~17 000 primes */
    uint32_t *primes = NULL;
    size_t prime_cnt = 0;
    generate_primes(PRIME_LIMIT, &primes, &prime_cnt);
    if (prime_cnt == 0) return 0;

    /* 2. Build heap with one entry per starting prime. */
    Node *heap = (Node *)calloc(prime_cnt, sizeof(Node));
    size_t heap_size = 0;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        Node node;
        node.next = (idx_t)(i + 1);
        seq_t sq   = ((seq_t)primes[i]) * ((seq_t)primes[i]);
        node.sum   = sq;
        heap_push(heap, &heap_size, node);
    }

    /* 3. Generate sums in ascending order, eliminating duplicates. */
    seq_t prev = 0;
    bool has_prev = false;
    idx_t rank = 0;
    seq_t result = 0;

    while (rank < n && heap_size > 0)
    {
        Node cur = heap_pop(heap, &heap_size);

        /* unique check */
        if (!has_prev || cur.sum != prev)
        {
            ++rank;
            if (rank == n)
            {
                result = cur.sum;
                break;
            }
            prev = cur.sum;
            has_prev = true;
        }

        /* extend the block if possible, taking care of overflow */
        if (cur.next < prime_cnt)
        {
            seq_t add = ((seq_t)primes[cur.next]) * ((seq_t)primes[cur.next]);
            if (!would_overflow_sum(cur.sum, add))
            {
                cur.sum += add;
                cur.next += 1;
                heap_push(heap, &heap_size, cur);
            }
        }
    }

    /* 4. Clean up */
    free(primes);
    free(heap);

    return result;
}
