/*
 * A359008  Jane Street's infinite sidewalk's greedy walk inverse mapping.
 * https://oeis.org/A359008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = unique i with A359005(i) = n; found by simulating the
 *            greedy walk A359005 (a(0)=0; back = c - floor(c/2) - 1 if
 *            unvisited, else c + floor(c/2) + 1) until the value n appears
 * Range:     u8 n <= 0, u16 n <= 2471, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9434 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static uint64_t *inv_idx = NULL;   /* inv_idx[v] = step at which v was visited, or ~0 */
static size_t inv_cap = 0;
static seq_t *inv_big = NULL;      /* visited values >= inv_cap */
static size_t inv_big_len = 0, inv_big_cap = 0;
static seq_t inv_curr;
static uint64_t inv_pos = 0;

static void inv_reset(size_t cap)
{
    if (inv_idx) free(inv_idx);
    if (inv_big) mem_free(inv_big);
    inv_big = NULL; inv_big_len = 0; inv_big_cap = 0;
    inv_cap = cap;
    inv_idx = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (size_t i = 0; i < cap; ++i) inv_idx[i] = ~(uint64_t)0;
    inv_idx[0] = 0;
    inv_curr = (seq_t)0;
    inv_pos = 0;
}

static bool inv_visited(seq_t v)
{
    if (v < (seq_t)inv_cap) return inv_idx[(size_t)(long long)v] != ~(uint64_t)0;
    for (size_t i = 0; i < inv_big_len; ++i)
        if (inv_big[i] == v) return true;
    return false;
}

static void inv_mark(seq_t v, uint64_t pos)
{
    if (v < (seq_t)inv_cap) {
        inv_idx[(size_t)(long long)v] = pos;
        return;
    }
    if (inv_big_len >= inv_big_cap) {
        size_t nc = inv_big_cap ? inv_big_cap * 2 : 64;
        seq_t *nb = mem_new(seq_t, nc);
        for (size_t i = 0; i < inv_big_len; ++i) nb[i] = inv_big[i];
        if (inv_big) mem_free(inv_big);
        inv_big = nb;
        inv_big_cap = nc;
    }
    inv_big[inv_big_len++] = v;
}

seq_t a(idx_t n)
{
    if (inv_idx == NULL || (seq_t)n >= (seq_t)inv_cap) {
        size_t cap = (size_t)n * 2 + 4096;
        if (inv_idx != NULL && cap < inv_cap * 2) cap = inv_cap * 2;
        inv_reset(cap);
    }
    while (inv_idx[(size_t)n] == ~(uint64_t)0) {
        seq_t c = inv_curr;
        seq_t next = c + c / (seq_t)2 + (seq_t)1;
        if (c > (seq_t)0) {
            seq_t back = c - c / (seq_t)2 - (seq_t)1;
            if (!inv_visited(back)) next = back;
        }
        inv_pos++;
        inv_curr = next;
        inv_mark(next, inv_pos);
    }
    return (seq_t)inv_idx[(size_t)n];
}
