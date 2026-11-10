/*
 * A276787  Repeated numbers in A276786.
 * https://oeis.org/A276787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th distinct value occurring at least twice in the
 *            sorted multiset generated from 1 by m -> 2m+1, 3m+1 (A276786);
 *            computed with a min-heap, counting runs of equal values
 * Range:     u8 n <= 2, u16 n <= 53, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9525 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A276787_heap = NULL;
static size_t A276787_hsz = 0, A276787_hcap = 0;
static seq_t *A276787_cache = NULL;
static size_t A276787_csz = 0, A276787_ccap = 0;

static void A276787_push(seq_t v) {
    if (A276787_hsz == A276787_hcap) {
        A276787_hcap = A276787_hcap ? A276787_hcap << 1 : 16;
        A276787_heap = mem_resize(seq_t, A276787_heap, A276787_hcap);
    }
    size_t i = A276787_hsz++;
    A276787_heap[i] = v;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (A276787_heap[p] <= A276787_heap[i]) break;
        seq_t t = A276787_heap[p]; A276787_heap[p] = A276787_heap[i]; A276787_heap[i] = t;
        i = p;
    }
}

static seq_t A276787_pop(void) {
    seq_t v = A276787_heap[0];
    --A276787_hsz;
    if (A276787_hsz) {
        A276787_heap[0] = A276787_heap[A276787_hsz];
        size_t i = 0;
        while (1) {
            size_t l = (i << 1) + 1;
            if (l >= A276787_hsz) break;
            size_t r = l + 1, s = l;
            if (r < A276787_hsz && A276787_heap[r] < A276787_heap[l]) s = r;
            if (A276787_heap[s] >= A276787_heap[i]) break;
            seq_t t = A276787_heap[s]; A276787_heap[s] = A276787_heap[i]; A276787_heap[i] = t;
            i = s;
        }
    }
    return v;
}

static bool A276787_started = false;

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)0;
    size_t need = (size_t)n;
    if (A276787_csz < need) {
        if (A276787_ccap < need) {
            size_t nc = A276787_ccap ? A276787_ccap << 1 : 16;
            while (nc < need) nc <<= 1;
            A276787_cache = mem_resize(seq_t, A276787_cache, nc);
            A276787_ccap = nc;
        }
        if (!A276787_started) {
            A276787_started = true;
            A276787_push((seq_t)1);
        }
        while (A276787_csz < need) {
            seq_t v = A276787_pop();
            A276787_push(v * (seq_t)2 + (seq_t)1);
            A276787_push(v * (seq_t)3 + (seq_t)1);
            size_t cnt = 1;
            while (A276787_hsz && A276787_heap[0] == v) {
                A276787_pop();
                A276787_push(v * (seq_t)2 + (seq_t)1);
                A276787_push(v * (seq_t)3 + (seq_t)1);
                cnt++;
            }
            if (cnt >= 2) A276787_cache[A276787_csz++] = v;
        }
    }
    return A276787_cache[need - 1];
}
