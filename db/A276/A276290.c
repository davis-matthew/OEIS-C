/*
 * A276290  Products of odd primes p and q such that either p or q is in the
 *          trajectory of (p*q)+1 under the Collatz 3x+1 map (A014682).
 * https://oeis.org/A276290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 3596, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   collatz_hits, next_prime, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A276290:  Enumerate the products p*q of odd primes
   (p≠3) such that either p or q occurs in the Collatz trajectory
   of p*q+1.  The values are produced in ascending order by
   maintaining a min‑heap of prime pairs sorted by p*q.  The
   implementation grows the prime table dynamically on demand. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/next_prime.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/collatz_hits.h"

typedef struct { idx_t p_idx; idx_t q_idx; } Pair;

/* -------------------- static data -------------------- */
static uint64_t *primes = NULL;          /* list of odd primes, 5,7,11,... */
static size_t  primes_cnt   = 0;
static size_t  primes_cap   = 0;

static Pair  *heap  = NULL;              /* min‑heap of pairs */
static size_t  heap_cnt  = 0;
static size_t  heap_cap  = 0;

static seq_t  *results = NULL;           /* generated sequence */
static size_t  results_cnt = 0;
static size_t  results_cap = 0;

/* -------------------- helper functions -------------------- */
static inline unsigned __int128 pair_product(idx_t i, idx_t j)
{
    return (unsigned __int128)primes[i] * (unsigned __int128)primes[j];
}

/* Expand a plain array of uint64_t */
static void grow_primes_cap(size_t needed)
{
    if (primes_cap < needed) {
        size_t n = primes_cap ? primes_cap * 2 : 8;
        while (n < needed) n <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(primes, n * sizeof(uint64_t));
        primes = tmp;
        primes_cap = n;
    }
}

/* Expand a plain array of Pair */
static void grow_heap_cap(size_t needed)
{
    if (heap_cap < needed) {
        size_t n = heap_cap ? heap_cap * 2 : 8;
        while (n < needed) n <<= 1;
        Pair *tmp = (Pair *)realloc(heap, n * sizeof(Pair));
        heap = tmp;
        heap_cap = n;
    }
}

/* Expand the results array (seq_t) */
static void grow_results_cap(size_t needed)
{
    if (results_cap < needed) {
        size_t n = results_cap ? results_cap * 2 : 8;
        while (n < needed) n <<= 1;
        if (results == NULL) {
            results = seq_alloc(n);
        } else {
            results = seq_realloc(results, results_cap, n);
        }
        results_cap = n;
    }
}

static void add_prime(uint64_t p)
{
    grow_primes_cap(primes_cnt + 1);
    primes[primes_cnt++] = p;
}

static void heap_push(Pair node)
{
    grow_heap_cap(heap_cnt + 1);
    size_t pos = heap_cnt++;
    heap[pos] = node;
    /* sift up */
    while (pos > 0) {
        size_t parent = (pos - 1) >> 1;
        if (pair_product(heap[pos].p_idx, heap[pos].q_idx) >=
            pair_product(heap[parent].p_idx, heap[parent].q_idx))
            break;
        Pair t = heap[pos];
        heap[pos] = heap[parent];
        heap[parent] = t;
        pos = parent;
    }
}

static Pair heap_pop(void)
{
    Pair top = heap[0];
    heap[0] = heap[--heap_cnt];
    size_t pos = 0;
    while (1) {
        size_t left = (pos << 1) + 1;
        if (left >= heap_cnt) break;
        size_t right = left + 1;
        size_t small = left;
        if (right < heap_cnt &&
            pair_product(heap[right].p_idx, heap[right].q_idx) <
            pair_product(heap[left].p_idx, heap[left].q_idx))
            small = right;
        if (pair_product(heap[pos].p_idx, heap[pos].q_idx) <=
            pair_product(heap[small].p_idx, heap[small].q_idx))
            break;
        Pair t = heap[pos];
        heap[pos] = heap[small];
        heap[small] = t;
        pos = small;
    }
    return top;
}

static void add_result(seq_t val)
{
    grow_results_cap(results_cnt + 1);
    results[results_cnt++] = val;
}

/* -------------------- main sequence function -------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        /* first odd prime > 3 is 5 */
        add_prime(5);
        heap_push((Pair){0, 0});   /* start with (5,5) */
        initialized = true;
    }

    /* Generate terms until we have at least n results. */
    while (results_cnt < n) {
        if (heap_cnt == 0) /* should not happen */
            return 0;

        Pair node = heap_pop();
        uint64_t p = primes[node.p_idx];
        uint64_t q = primes[node.q_idx];

        uint64_t N = p * q + 1;
        if (collatz_hits(p, q, N))
            add_result((seq_t)p * (seq_t)q);

        /* Schedule next pair for the same p. */
        size_t next_q = node.q_idx + 1;
        if (next_q >= primes_cnt) {
            /* Need a new prime: generate it lazily. */
            uint64_t next_p = next_prime(primes[primes_cnt - 1] + 1);
            add_prime(next_p);
            /* Add its diagonal pair so it will pair with later primes. */
            heap_push((Pair){primes_cnt - 1, primes_cnt - 1});
        }
        heap_push((Pair){node.p_idx, next_q});
    }

    return results[n - 1];
}
