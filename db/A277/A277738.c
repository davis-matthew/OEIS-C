/*
 * A277738  Positions of 2's in A277735.
 * https://oeis.org/A277738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 2 in A277735, the fixed point of
 *            0->01, 1->20, 2->0
 * Range:     u8 n <= 41, u16 n <= 10533, u32 n <= 10609, u64 n <= 10609,
 *            u128 n <= 10609, big any n
 * Validated: standard, pass, 80 terms through n = 9959 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static unsigned char *A277738_w = NULL;
static size_t A277738_len = 0, A277738_cap = 0, A277738_gen = 0;
static uint64_t *A277738_pos = NULL;
static size_t A277738_cnt = 0, A277738_pcap = 0;

static void A277738_push(unsigned char c) {
    if (A277738_len == A277738_cap) {
        size_t nc = A277738_cap ? A277738_cap * 2 : 1024;
        A277738_w = (unsigned char *)realloc(A277738_w, nc);
        A277738_cap = nc;
    }
    A277738_w[A277738_len++] = c;
    if (c == 2) {
        if (A277738_cnt == A277738_pcap) {
            size_t nc = A277738_pcap ? A277738_pcap * 2 : 1024;
            A277738_pos = (uint64_t *)realloc(A277738_pos, nc * sizeof(uint64_t));
            A277738_pcap = nc;
        }
        A277738_pos[A277738_cnt++] = (uint64_t)A277738_len; /* 1-based */
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (A277738_len == 0) {
        A277738_push(0);
        A277738_push(1);
        A277738_gen = 1;
    }
    while ((idx_t)A277738_cnt < n) {
        unsigned char c = A277738_w[A277738_gen++];
        if (c == 0) { A277738_push(0); A277738_push(1); }
        else if (c == 1) { A277738_push(2); A277738_push(0); }
        else { A277738_push(0); }
    }
    return (seq_t)A277738_pos[n - 1];
}
