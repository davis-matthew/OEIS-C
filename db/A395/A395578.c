/*
 * A395578  Squarefree kernel of A303606(n).
 * https://oeis.org/A395578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A007947(A303606(n)) = the squarefree composite base m of
 *            the n-th term m^k (k>=2) of A303606; computed by merging powers
 *            of composite squarefree bases with a min-heap
 * Range:     u8 n <= 4, u16 n <= 900, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9784 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static bool A395578_csf(uint64_t n)
{
    if (n < 6) return false;
    uint64_t t = n;
    int primes = 0;
    if (t % 2 == 0) {
        primes++;
        t /= 2;
        if (t % 2 == 0) return false;
    }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            primes++;
            t /= p;
            if (t % p == 0) return false;
        }
    }
    if (t > 1) primes++;
    return primes >= 2;
}

typedef struct {
    seq_t val;
    uint64_t base;
} A395578_node;

static A395578_node *A395578_heap = NULL;
static size_t A395578_hsize = 0, A395578_hcap = 0;
static uint64_t *A395578_cache = NULL;
static size_t A395578_csize = 0, A395578_ccap = 0;
static uint64_t A395578_next = 6;

static void A395578_push(seq_t val, uint64_t base)
{
    if (A395578_hsize == A395578_hcap) {
        size_t nc = A395578_hcap ? A395578_hcap * 2 : 16;
        A395578_heap = mem_resize(A395578_node, A395578_heap, nc);
        A395578_hcap = nc;
    }
    size_t i = A395578_hsize++;
    A395578_heap[i].val = val;
    A395578_heap[i].base = base;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (A395578_heap[p].val <= A395578_heap[i].val) break;
        A395578_node t = A395578_heap[i];
        A395578_heap[i] = A395578_heap[p];
        A395578_heap[p] = t;
        i = p;
    }
}

static A395578_node A395578_pop(void)
{
    A395578_node ret = A395578_heap[0];
    A395578_hsize--;
    if (A395578_hsize == 0) return ret;
    A395578_heap[0] = A395578_heap[A395578_hsize];
    size_t i = 0;
    while (1) {
        size_t l = 2 * i + 1, r = l + 1;
        if (l >= A395578_hsize) break;
        size_t s = l;
        if (r < A395578_hsize && A395578_heap[r].val < A395578_heap[l].val) s = r;
        if (A395578_heap[i].val <= A395578_heap[s].val) break;
        A395578_node t = A395578_heap[i];
        A395578_heap[i] = A395578_heap[s];
        A395578_heap[s] = t;
        i = s;
    }
    return ret;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A395578_csize < (size_t)n) {
        if (A395578_hsize == 0) {
            while (!A395578_csf(A395578_next)) A395578_next++;
            A395578_push((seq_t)A395578_next * (seq_t)A395578_next, A395578_next);
            A395578_next++;
        }
        seq_t topv = A395578_heap[0].val;
        while (1) {
            uint64_t cand = A395578_next;
            while (!A395578_csf(cand)) cand++;
            seq_t sq = (seq_t)cand * (seq_t)cand;
            if (sq > topv) break;
            A395578_push(sq, cand);
            A395578_next = cand + 1;
        }
        A395578_node cur = A395578_pop();
        if (A395578_csize == A395578_ccap) {
            size_t nc = A395578_ccap ? A395578_ccap * 2 : 16;
            A395578_cache = mem_resize(uint64_t, A395578_cache, nc);
            A395578_ccap = nc;
        }
        A395578_cache[A395578_csize++] = cur.base;
        seq_t nxt = cur.val * (seq_t)cur.base;
        if (nxt / (seq_t)cur.base == cur.val)
            A395578_push(nxt, cur.base);
    }
    return (seq_t)A395578_cache[n - 1];
}
