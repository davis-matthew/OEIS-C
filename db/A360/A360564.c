/*
 * A360564  Numerators of breadth-first numerator-denominator-incrementing
 *          enumeration of rationals in (0,1).
 * https://oeis.org/A360564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9659, u16 n <= 9659, u32 n <= 9659, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A360564 – numerators of the breadth‑first enumeration of
   rationals in (0,1).  
   The algorithm starts with 1/2 and repeatedly adds, in breadth‑first
   order, the children p/(q+1) (always < 1) and (p+1)/q
   (if p+1 < q).  Each distinct reduced fraction appears only once.
   a(n) is the numerator of the n‑th fraction (1‑based). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* Helper: integer gcd for idx_t (uint64_t)                           */
/* ------------------------------------------------------------------ */
static inline uint64_t igcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ------------------------------------------------------------------ */
/* Visited set – open addressing with linear probing.
   Size is a power of two large enough for thousands of nodes.       */
/* ------------------------------------------------------------------ */
#define VISIT_CAP (1 << 20)               /* 1 048 576 */

/* encode pair into 64‑bit key assuming each component < 2³²           */
static inline uint64_t pair_key(uint64_t p, uint64_t q)
{
    return (p << 32) | q;
}

static uint64_t visited[VISIT_CAP];   /* 0 means empty */

/* linear probing search */
static inline bool visited_contains(uint64_t key)
{
    size_t idx = key & (VISIT_CAP - 1);
    while (visited[idx]) {
        if (visited[idx] == key) return true;
        idx = (idx + 1) & (VISIT_CAP - 1);
    }
    return false;
}

static inline void visited_add(uint64_t key)
{
    size_t idx = key & (VISIT_CAP - 1);
    while (visited[idx]) idx = (idx + 1) & (VISIT_CAP - 1);
    visited[idx] = key;
}

/* ------------------------------------------------------------------ */
/* Queue for breadth‑first traversal                                   */
/* ------------------------------------------------------------------ */
static uint64_t *queue_num = NULL;
static uint64_t *queue_den = NULL;
static size_t queue_cap = 0;
static size_t q_head = 0, q_tail = 0;

static void ensure_queue_cap(size_t need)
{
    if (need <= queue_cap) return;
    size_t new_cap = (queue_cap == 0) ? 128 : queue_cap;
    while (need > new_cap) new_cap <<= 1;
    uint64_t *tmp_num = (uint64_t*)realloc(queue_num, new_cap * sizeof(uint64_t));
    uint64_t *tmp_den = (uint64_t*)realloc(queue_den, new_cap * sizeof(uint64_t));
    queue_num = tmp_num;
    queue_den = tmp_den;
    queue_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Cache of numerators of visited nodes                                */
/* ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;      /* numerators in order of visit   */
static size_t cache_cap = 0;
static size_t cache_len = 0;          /* number of terms cached       */

/* enlarge cache when required */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap;
    while (need > new_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Reduce a fraction a/b to lowest terms (returns np,nq).              */
/* ------------------------------------------------------------------ */
static inline void reduce_pair(uint64_t a, uint64_t b, uint64_t *np, uint64_t *nq)
{
    uint64_t g = igcd(a, b);
    *np = a / g;
    *nq = b / g;
}

/* ------------------------------------------------------------------ */
/* Main sequence routine                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; protect against overflow */

    /* initialize once */
    if (seq_cache == NULL) {
        ensure_queue_cap(1);
        queue_num[0] = 1;
        queue_den[0] = 2;
        q_head = 0;
        q_tail = 1;
        visited_add(pair_key(1, 2));
    }

    /* expand cache until we have at least n terms */
    while (cache_len < n) {
        /* queue should never be empty for valid n */
        uint64_t p = queue_num[q_head];
        uint64_t q = queue_den[q_head];
        ++q_head;

        /* cache numerator */
        ensure_cache_cap(cache_len + 1);
        seq_cache[cache_len] = (seq_t)p;
        ++cache_len;

        /* child 1: p/(q+1) – always < 1 */
        uint64_t np1, nq1;
        reduce_pair(p, q + 1, &np1, &nq1);
        uint64_t key1 = pair_key(np1, nq1);
        if (!visited_contains(key1)) {
            visited_add(key1);
            ensure_queue_cap(q_tail + 1);
            queue_num[q_tail] = np1;
            queue_den[q_tail] = nq1;
            ++q_tail;
        }

        /* child 2: (p+1)/q – add only if < 1 */
        if (p + 1 < q) {
            uint64_t np2, nq2;
            reduce_pair(p + 1, q, &np2, &nq2);
            uint64_t key2 = pair_key(np2, nq2);
            if (!visited_contains(key2)) {
                visited_add(key2);
                ensure_queue_cap(q_tail + 1);
                queue_num[q_tail] = np2;
                queue_den[q_tail] = nq2;
                ++q_tail;
            }
        }
    }

    return seq_cache[n - 1];
}
