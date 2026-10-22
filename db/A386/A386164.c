/*
 * A386164  Primes having only {2, 5, 8, 9} as digits.
 * https://oeis.org/A386164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime number whose decimal digits are all in
 *            {2,5,8,9}
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 42, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"

/* static state for memoisation */
static bool initialized = false;

static seq_t *prime_cache = NULL;      /* array of primes that have been found */
static size_t prime_cnt = 0;           /* number of primes stored */
static size_t prime_cap = 0;           /* capacity of prime_cache */

/* min‑heap of candidate numbers */
static seq_t *heap = NULL;
static size_t heap_cnt = 0;
static size_t heap_cap = 0;

static const seq_t allowed[4] = { (seq_t)2, (seq_t)5, (seq_t)8, (seq_t)9 };

/* Grow heap pool if more space is required */
static void heap_grow(size_t needed)
{
    if (needed <= heap_cap)
        return;
    size_t new_cap = heap_cap ? (heap_cap << 1) : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    heap = seq_realloc(heap, heap_cap, new_cap);
    heap_cap = new_cap;
}

/* Push a value into the min‑heap */
static void heap_push(seq_t val)
{
    heap_grow(heap_cnt + 1);
    heap[heap_cnt++] = val;
    size_t i = heap_cnt - 1;
    while (i > 0) {
        size_t parent = (i - 1) >> 1;
        if (heap[i] < heap[parent]) {
            seq_t tmp = heap[i];
            heap[i] = heap[parent];
            heap[parent] = tmp;
            i = parent;
        } else
            break;
    }
}

/* Pop the smallest value from the min‑heap */
static seq_t heap_pop(void)
{
    seq_t min = heap[0];
    if (--heap_cnt > 0) {
        heap[0] = heap[heap_cnt];
        size_t i = 0;
        while (true) {
            size_t left = (i << 1) + 1;
            size_t right = left + 1;
            if (left >= heap_cnt)
                break;
            size_t smallest = left;
            if (right < heap_cnt && heap[right] < heap[left])
                smallest = right;
            if (heap[smallest] < heap[i]) {
                seq_t tmp = heap[i];
                heap[i] = heap[smallest];
                heap[smallest] = tmp;
                i = smallest;
            } else
                break;
        }
    }
    return min;
}

/* Grow prime cache if more space is required */
static void prime_grow(size_t needed)
{
    if (needed <= prime_cap)
        return;
    size_t new_cap = prime_cap ? (prime_cap << 1) : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    prime_cache = seq_realloc(prime_cache, prime_cap, new_cap);
    prime_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise heap with single‑digit candidates */
        heap_grow(16);
        for (size_t i = 0; i < 4; ++i)
            heap_push(allowed[i]);
        initialized = true;
    }

    while (prime_cnt < (size_t)n) {
        seq_t cur = heap_pop();

        /* generate successors of cur */
        for (size_t i = 0; i < 4; ++i) {
            seq_t nxt = cur * (seq_t)10 + allowed[i];
            heap_push(nxt);
        }

        if (is_prime_big(cur)) {
            prime_grow(prime_cnt + 1);
            prime_cache[prime_cnt++] = cur;
        }
    }

    return prime_cache[n - 1];
}
