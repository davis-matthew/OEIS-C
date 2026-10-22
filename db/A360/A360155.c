/*
 * A360155  Primes of the form m^2 + 2*(k+1)^2 such that m^2 + 2*k^2 is also
 *          prime.
 * https://oeis.org/A360155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the sorted list of the larger prime in each pair
   (m^2 + 2*k^2, m^2 + 2*(k+1)^2) whose two values are both prime.
   All admissible pairs satisfy m ≡ 3 (mod 6) and k ≡ 1 (mod 3). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

typedef struct {
    seq_t val;           /* the larger prime m^2 + 2*(k+1)^2          */
    uint64_t m;          /* current m                               */
    uint64_t k;          /* current k (≡1 mod 3) that produced val   */
} Node;

typedef struct {
    Node *data;
    size_t size;
    size_t cap;
} MinHeap;

/* ---------- heap helpers ---------- */
static void heap_push(MinHeap *h, Node n) {
    if (h->size == h->cap) {
        h->cap = h->cap * 2 + 4;
        h->data = (Node *)realloc(h->data, h->cap * sizeof(Node));
    }
    size_t i = h->size++;
    h->data[i] = n;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (h->data[p].val <= h->data[i].val) break;
        Node tmp = h->data[p];
        h->data[p] = h->data[i];
        h->data[i] = tmp;
        i = p;
    }
}

static Node heap_pop(MinHeap *h) {
    Node res = h->data[0];
    h->data[0] = h->data[--h->size];
    size_t i = 0;
    while (true) {
        size_t l = (i << 1) + 1;
        if (l >= h->size) break;
        size_t r = l + 1;
        size_t s = (r < h->size && h->data[r].val < h->data[l].val) ? r : l;
        if (h->data[i].val <= h->data[s].val) break;
        Node tmp = h->data[i];
        h->data[i] = h->data[s];
        h->data[s] = tmp;
        i = s;
    }
    return res;
}

/* ---------- prime pair finders ---------- */
static Node find_first_pair(uint64_t m) {
    seq_t msq = (seq_t)m * (seq_t)m;
    for (uint64_t k = 1;; k += 3) {
        seq_t ksq = (seq_t)k * (seq_t)k;
        seq_t p1 = msq + (seq_t)2 * ksq;
        uint64_t kp1 = k + 1;
        seq_t kp1_sq = (seq_t)kp1 * (seq_t)kp1;
        seq_t p2 = msq + (seq_t)2 * kp1_sq;
        if (is_prime_big(p1) && is_prime_big(p2)) {
            Node n = {p2, m, k};
            return n;
        }
    }
}

static Node find_next_pair(uint64_t m, uint64_t k_cur) {
    seq_t msq = (seq_t)m * (seq_t)m;
    uint64_t k = k_cur + 3;              /* skip to next admissible k   */
    while (true) {
        seq_t ksq = (seq_t)k * (seq_t)k;
        seq_t p1 = msq + (seq_t)2 * ksq;
        uint64_t kp1 = k + 1;
        seq_t kp1_sq = (seq_t)kp1 * (seq_t)kp1;
        seq_t p2 = msq + (seq_t)2 * kp1_sq;
        if (is_prime_big(p1) && is_prime_big(p2)) {
            Node n = {p2, m, k};
            return n;
        }
        k += 3;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n) {
    /* static state for incremental generation */
    static bool inited = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static MinHeap heap;
    static uint64_t next_m = 3;   /* next m to add (≡3 mod 6) */

    if (!inited) {
        /* initial cache capacity */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;

        /* initialise heap */
        heap.cap = 64;
        heap.size = 0;
        heap.data = (Node *)malloc(heap.cap * sizeof(Node));

        /* push first admissible m = 3 */
        heap_push(&heap, find_first_pair(next_m));
        next_m += 6;

        inited = true;
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* heap must not be empty */
        if (heap.size == 0) {
            heap_push(&heap, find_first_pair(next_m));
            next_m += 6;
        }

        /* pop smallest prime */
        Node cur = heap_pop(&heap);

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = cur.val;

        /* generate next pair for same m */
        Node nxt = find_next_pair(cur.m, cur.k);
        heap_push(&heap, nxt);

        /* add a new m */
        Node newm = find_first_pair(next_m);
        heap_push(&heap, newm);
        next_m += 6;
    }

    return cache[n - 1];   /* sequence is 1‑based */
}
