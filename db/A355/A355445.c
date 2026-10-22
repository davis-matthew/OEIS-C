/*
 * A355445  Numbers of the form p^2 * q where p and q are primes with p^2 < q.
 * https://oeis.org/A355445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A355445: Numbers of the form p^2 * q with p^2 < q,
   where p and q are primes.  The function returns the nth such number
   (one‑based offset).  It generates primes up to a fixed limit once
   and then uses a min‑heap to produce the terms in ascending order. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

typedef struct {
    seq_t  val;   /* p^2 * q */
    size_t p_idx; /* index of p in the primes array */
    size_t q_idx; /* index of q in the primes array */
} HeapNode;

static void sift_up(HeapNode *heap, size_t idx, size_t heap_size)
{
    while (idx > 0) {
        size_t parent = (idx - 1) >> 1;
        if (heap[idx].val < heap[parent].val) {
            HeapNode tmp = heap[idx];
            heap[idx] = heap[parent];
            heap[parent] = tmp;
            idx = parent;
        } else {
            break;
        }
    }
}

static void sift_down(HeapNode *heap, size_t idx, size_t heap_size)
{
    while (1) {
        size_t left   = (idx << 1) + 1;
        size_t right  = left + 1;
        size_t smallest = idx;

        if (left < heap_size && heap[left].val < heap[smallest].val)
            smallest = left;
        if (right < heap_size && heap[right].val < heap[smallest].val)
            smallest = right;

        if (smallest == idx)
            break;
        HeapNode tmp = heap[idx];
        heap[idx] = heap[smallest];
        heap[smallest] = tmp;
        idx = smallest;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* Static data that survives the first call */
    static bool          initialized = false;
    static uint64_t     *primes      = NULL;   /* array of primes       */
    static size_t        prime_cnt   = 0;      /* number of primes      */
    static HeapNode      *heap        = NULL;   /* priority queue        */
    static size_t        heap_size   = 0;      /* current heap size     */
    static size_t        heap_cap    = 0;      /* heap capacity         */
    static seq_t        *terms       = NULL;   /* cached sequence values*/
    static size_t        terms_cnt   = 0;      /* terms generated       */
    static size_t        terms_cap   = 0;      /* capacity of terms arr */

    if (!initialized) {
        /* --------- Generate an initial pool of primes --------- */
        const idx_t LIMIT = 2000000;   /* enough for the first ~10000 terms */
        build_primes_upto(LIMIT, &primes, &prime_cnt);

        /* --------- Build the initial heap of candidates --------- */
        heap_cap = prime_cnt;
        heap = (HeapNode *)malloc(heap_cap * sizeof(HeapNode));
        heap_size = 0;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p  = primes[i];
            uint64_t p2 = p * p;                /* p^2 */
            /* find first prime q > p^2 */
            size_t lo = 0, hi = prime_cnt;
            while (lo < hi) {
                size_t mid = (lo + hi) >> 1;
                if (primes[mid] > p2)
                    hi = mid;
                else
                    lo = mid + 1;
            }
            if (lo == prime_cnt)   /* no suitable q, skip this p */
                continue;
            seq_t val = (seq_t)p * (seq_t)p * (seq_t)primes[lo];
            heap[heap_size++] = (HeapNode){val, i, lo};
        }

        /* Heapify */
        for (size_t i = heap_size / 2; i > 0; --i)
            sift_down(heap, i - 1, heap_size);

        /* --------- Prepare space for cached terms --------- */
        terms_cap = 256;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));

        initialized = true;
    }

    /* --------- Generate terms up to index n --------- */
    while (terms_cnt < (size_t)n) {
        if (heap_size == 0)
            break;   /* should not happen for required n */

        /* Extract the smallest candidate */
        HeapNode top = heap[0];
        heap[0] = heap[--heap_size];
        sift_down(heap, 0, heap_size);

        /* Store the extracted value */
        if (terms_cnt >= terms_cap) {
            terms_cap <<= 1;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_cnt++] = top.val;

        /* Insert the next candidate for the same prime p */
        size_t p_idx = top.p_idx;
        size_t q_idx = top.q_idx + 1;
        if (q_idx < prime_cnt) {
            uint64_t p  = primes[p_idx];
            seq_t val = (seq_t)p * (seq_t)p * (seq_t)primes[q_idx];
            heap[heap_size++] = (HeapNode){val, p_idx, q_idx};
            sift_up(heap, heap_size - 1, heap_size);
        }
    }

    return terms[(size_t)n - 1];
}
