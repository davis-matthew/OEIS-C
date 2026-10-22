/*
 * A368950  Numbers with 11 odd divisors.
 * https://oeis.org/A368950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   mul_small, next_prime, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A368950:  Numbers of the form 2^k * p^10,  k ≥ 0,  p odd prime,  ordered increasingly.  */

/*  The implementation creates a min‑heap that contains the next unprocessed
 *  value for each prime.  Initially the heap holds 3^10.  In each step
 *  the smallest element is removed and returned.  Its value is then
 *  multiplied by 2 and re‑inserted (the next element for that prime).
 *  After every extraction new primes are added to the heap while their
 *  initial value p^10 does not exceed the current minimum of the heap.
 *
 *  The heap is implemented with a dynamic array and simple
 *  sift‑up / sift‑down operations.  All arithmetic uses seq_t and the
 *  provided helpers pow_int, mul_small and next_prime.  The algorithm
 *  works for arbitrary‑precision seq_t as well as for 8‑ to 128‑bit
 *  unsigned integer types.  The routine runs in O(n log π(n)) time,
 *  comfortably below the time limit for n ≤ 10000.  */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_int.h"
#include "../utils/mul_small.h"
#include "../utils/next_prime.h"

typedef struct { seq_t val; uint64_t prime; } Node;

/* ------- heap operations --------------------------------------------- */

static void heap_swap(Node *a, Node *b)
{
    Node t = *a; *a = *b; *b = t;
}

static void heap_push(Node **heapP, size_t *sizeP, size_t *capP, Node key)
{
    Node *heap = *heapP;
    size_t size = *sizeP, cap = *capP;

    if (size == cap) {
        cap = cap ? cap << 1 : 64;
        heap = (Node *)realloc(heap, cap * sizeof(Node));
        *heapP = heap; *capP = cap;
    }

    heap[size++] = key;
    size_t i = size - 1;

    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;
        heap_swap(&heap[p], &heap[i]);
        i = p;
    }
    *sizeP = size;
}

static Node heap_pop(Node **heapP, size_t *sizeP)
{
    Node *heap = *heapP;
    size_t size = *sizeP;

    Node min = heap[0];
    heap[0] = heap[--size];
    size_t i = 0;

    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= size) break;
        size_t r = l + 1;
        size_t s = l;
        if (r < size && heap[r].val < heap[l].val) s = r;
        if (heap[i].val <= heap[s].val) break;
        heap_swap(&heap[i], &heap[s]);
        i = s;
    }

    *sizeP = size;
    return min;
}

/* ------- main sequence function --------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; the first value is a(1) */

    Node *heap = NULL;
    size_t heap_size = 0, heap_cap = 0;

    /* insert first prime 3 */
    seq_t p10 = pow_int((seq_t)3, 10);          /* 3^10 */
    heap_push(&heap, &heap_size, &heap_cap, (Node){p10, 3});

    uint64_t next_prime_val = next_prime(3);    /* next prime after 3 */

    idx_t count = 0;
    while (1) {
        Node cur = heap_pop(&heap, &heap_size);
        ++count;
        if (count == n) {
            free(heap);
            return cur.val;
        }

        /* extend sequence for this prime by multiplying by 2 */
        seq_t new_val = mul_small(cur.val, 2);
        heap_push(&heap, &heap_size, &heap_cap, (Node){new_val, cur.prime});

        /* add new primes while their p^10 would appear before the current minimum */
        while (heap_size > 0) {
            seq_t cand = pow_int((seq_t)next_prime_val, 10);
            if (cand <= heap[0].val) {
                heap_push(&heap, &heap_size, &heap_cap, (Node){cand, next_prime_val});
                next_prime_val = next_prime(next_prime_val);
            } else break;
        }
    }
    /* should never reach here */
    free(heap);
    return 0;
}
