/*
 * A392423  Powers k^m, m > 1, odd k that are neither squarefree nor
 *          squareful.
 * https://oeis.org/A392423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 1118, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392423: Powers k^m (m>1) with odd k that are neither squarefree
   nor squareful.  For a non‑prime base k, both a prime factor of exponent 1
   and a prime factor of exponent ≥2 must occur.  Values are output in
   ascending order with the offset 1. */
/* The implementation uses a min‑heap to generate the required k^m in
   sorted order, and a simple cache to allow arbitrary calls to a(idx). */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions available in the build environment  */
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

/* Node used in the priority queue  */
typedef struct {
    seq_t   val;     /* current value k^m  */
    uint64_t base;   /* base k           */
    uint64_t exp;    /* current exponent m */
} node_t;

/* ---------- heap utilities ---------- */
static node_t *heap     = NULL;
static size_t  heap_sz  = 0;
static size_t  heap_cap = 0;

/* push a node into the min‑heap   */
static void heap_push(uint64_t base, uint64_t exp, seq_t val)
{
    if (heap_sz == heap_cap) {
        heap_cap = heap_cap ? heap_cap * 2 : 16;
        heap = (node_t *)realloc(heap, heap_cap * sizeof(node_t));
    }
    size_t i = heap_sz++;
    heap[i].base = base;
    heap[i].exp  = exp;
    heap[i].val  = val;

    /* sift up to maintain min‑heap property */
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= heap[i].val) break;          /* min‑heap */
        node_t tmp = heap[i];
        heap[i] = heap[p];
        heap[p] = tmp;
        i = p;
    }
}

/* pop the smallest node from the heap   */
static node_t heap_pop(void)
{
    node_t res = heap[0];
    heap[0] = heap[--heap_sz];
    /* sift down */
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        size_t r = l + 1;
        size_t smallest = i;
        if (l < heap_sz && heap[l].val < heap[smallest].val) smallest = l;
        if (r < heap_sz && heap[r].val < heap[smallest].val) smallest = r;
        if (smallest == i) break;
        node_t tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
    return res;
}

/* ---------- base validation ---------- */
static bool is_valid_base(uint64_t k)
{
    if ((k & 1u) == 0) return false; /* must be odd */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(k, primes, exps, &cnt);

    bool has1 = false, has2 = false;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] == 1) has1 = true;
        if (exps[i] >= 2) has2 = true;
        if (has1 && has2) return true;
    }
    return false;
}

/* ---------- sequence storage ---------- */
static seq_t *seq      = NULL;
static size_t seq_sz   = 0;
static size_t seq_cap  = 0;

static void append_seq_val(seq_t v)
{
    /* avoid duplicates */
    if (seq_sz > 0 && seq[seq_sz-1] == v) return;

    if (seq_sz == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 256;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
    }
    seq[seq_sz++] = v;
}

/* ---------- main function ---------- */
static uint64_t next_base = 3;   /* smallest odd candidate */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for offset 1 */

    /* generate terms until we have at least n of them */
    while (seq_sz < (size_t)n) {
        /* If heap empty, find and insert the first valid base */
        while (heap_sz == 0) {
            seq_t sq = big_pow((seq_t)next_base, (seq_t)2);
            if (is_valid_base(next_base))
                heap_push(next_base, 2, sq);
            next_base += 2; /* only odd numbers */
        }

        /* Current smallest value in the heap */
        seq_t limit = heap[0].val;

        /* Insert new bases whose square is <= the current limit */
        while (1) {
            seq_t sq = big_pow((seq_t)next_base, (seq_t)2);
            if (sq <= limit) {
                if (is_valid_base(next_base))
                    heap_push(next_base, 2, sq);
                next_base += 2;
            } else
                break;
        }

        /* Extract the smallest power */
        node_t cur = heap_pop();
        append_seq_val(cur.val);

        /* Push the next power of the same base, stop if it would overflow */
        seq_t next_val = seq_mul(cur.val, (seq_t)cur.base);
        if (!(next_val < cur.val))          /* overflow protection, safe for uint64_t */
            heap_push(cur.base, cur.exp + 1, next_val);
    }

    return seq[n - 1];
}
