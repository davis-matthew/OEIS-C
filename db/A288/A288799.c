/*
 * A288799  Inverse permutation to A288164.
 * https://oeis.org/A288799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = k such that A288164(k) = n; A288164 is built by a greedy
 *            search (smallest unused a(k) with a(k-2)*a(k) having >= 5
 *            distinct primes, a(1)=1, a(2)=2), extended until n appears
 * Range:     u8 n <= 15, u16 n <= 25000, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 75 terms through n = 9798 (2026-11-08)
 * Helpers:   gcd_u64, omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/omega.h"
#include "../utils/gcd_u64.h"

#define A288799_TAB 1048576u

static uint8_t *A288799_om = NULL;
static uint64_t *A288799_seq = NULL;
static size_t A288799_len = 0, A288799_cap = 0;
static uint32_t *A288799_pos = NULL;   /* pos[v] = index in A288164, 0 if unused */
static size_t A288799_pcap = 0;
static uint64_t A288799_lo = 1;        /* smallest possibly-unused value */

static int A288799_omega(uint64_t v)
{
    if (v < A288799_TAB) return A288799_om[v];
    return (int)omega(v);
}

static void A288799_init_tab(void)
{
    if (A288799_om) return;
    A288799_om = (uint8_t*)calloc(A288799_TAB, 1);
    if (!A288799_om) abort();
    for (uint32_t p = 2; p < A288799_TAB; ++p) {
        if (A288799_om[p] == 0) {
            for (uint32_t q = p; q < A288799_TAB; q += p) A288799_om[q]++;
        }
    }
}

static void A288799_grow_pos(uint64_t v)
{
    if (v < A288799_pcap) return;
    size_t cap = A288799_pcap ? A288799_pcap : 4096;
    while (cap <= v) cap <<= 1;
    uint32_t *t = (uint32_t*)realloc(A288799_pos, cap * sizeof(uint32_t));
    if (!t) abort();
    for (size_t i = A288799_pcap; i < cap; ++i) t[i] = 0;
    A288799_pos = t;
    A288799_pcap = cap;
}

static void A288799_push(uint64_t v)
{
    if (A288799_len == A288799_cap) {
        size_t cap = A288799_cap ? A288799_cap << 1 : 1024;
        uint64_t *t = (uint64_t*)realloc(A288799_seq, cap * sizeof(uint64_t));
        if (!t) abort();
        A288799_seq = t;
        A288799_cap = cap;
    }
    A288799_seq[A288799_len++] = v;
    A288799_grow_pos(v);
    A288799_pos[v] = (uint32_t)A288799_len;
}

static void A288799_step(void)
{
    if (A288799_len == 0) { A288799_push(1); A288799_push(2); return; }
    uint64_t prev = A288799_seq[A288799_len - 2];
    int op = A288799_omega(prev);
    uint64_t c = A288799_lo;
    for (;; ++c) {
        if (c < A288799_pcap && A288799_pos[c]) {
            if (c == A288799_lo) ++A288799_lo;
            continue;
        }
        uint64_t g = gcd_u64(prev, c);
        int cnt = op + A288799_omega(c) - A288799_omega(g);
        if (cnt >= 5) break;
    }
    A288799_push(c);
}

seq_t a(idx_t n)
{
    A288799_init_tab();
    uint64_t m = (uint64_t)n;
    if (m == 0) return (seq_t)0;
    for (;;) {
        if (m < A288799_pcap && A288799_pos[m]) return (seq_t)A288799_pos[m];
        A288799_step();
    }
}
