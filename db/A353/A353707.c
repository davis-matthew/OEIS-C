/*
 * A353707  n appears in A121216 a(n) steps earlier than expected.
 * https://oeis.org/A353707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - A225047(n), where A225047 is the inverse of A121216
 *            (A121216(1)=1, A121216(2)=2, A121216(n) = smallest unused
 *            positive integer coprime to A121216(n-2)); computed by
 *            generating A121216 until n appears
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9490 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A353707_vals = NULL;
static size_t A353707_len = 0, A353707_cap = 0;
static uint64_t *A353707_pos = NULL;   /* pos[v] = 1-based index of v, 0 if unseen */
static size_t A353707_pcap = 0;
static uint64_t A353707_lo = 1;        /* smallest possibly-unused value */

static uint64_t A353707_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void A353707_ensure_pos(size_t idx)
{
    if (idx < A353707_pcap) return;
    size_t nc = A353707_pcap ? A353707_pcap : 16;
    while (nc <= idx) nc *= 2;
    uint64_t *t = (uint64_t *)realloc(A353707_pos, nc * sizeof(uint64_t));
    for (size_t i = A353707_pcap; i < nc; ++i) t[i] = 0;
    A353707_pos = t;
    A353707_pcap = nc;
}

static void A353707_push(uint64_t v)
{
    if (A353707_len == A353707_cap) {
        A353707_cap = A353707_cap ? A353707_cap * 2 : 16;
        A353707_vals = (uint64_t *)realloc(A353707_vals, A353707_cap * sizeof(uint64_t));
    }
    A353707_vals[A353707_len++] = v;
    A353707_ensure_pos((size_t)v);
    A353707_pos[(size_t)v] = (uint64_t)A353707_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A353707_len == 0) {
        A353707_ensure_pos(16);
        A353707_push(1);
        A353707_push(2);
        A353707_lo = 3;
    }
    for (;;) {
        if ((size_t)n < A353707_pcap && A353707_pos[(size_t)n] != 0)
            return (seq_t)n - (seq_t)A353707_pos[(size_t)n];
        uint64_t pp = A353707_vals[A353707_len - 2];
        for (;;) {
            A353707_ensure_pos((size_t)A353707_lo);
            if (A353707_pos[(size_t)A353707_lo] == 0) break;
            ++A353707_lo;
        }
        uint64_t c = A353707_lo;
        for (;;) {
            A353707_ensure_pos((size_t)c);
            if (A353707_pos[(size_t)c] == 0 && A353707_gcd(c, pp) == 1) break;
            ++c;
        }
        A353707_push(c);
    }
}
