/*
 * A280409  Primes in the order that they appear in A280408, without
 *          repetitions.
 * https://oeis.org/A280409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th distinct prime in order of first appearance in the
 *            Collatz trajectories of 1,2,3,... (simulation with a hash set
 *            for de-duplication)
 * Range:     u8 n <= 19, u16 n <= 467, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8011 (2026-11-08)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

static uint64_t *A280409_list = NULL;
static size_t A280409_len = 0;
static size_t A280409_cap = 0;

static uint64_t *A280409_tab = NULL;
static size_t A280409_tcap = 0;   /* power of two */
static size_t A280409_tcnt = 0;
static uint64_t A280409_next = 1;

static size_t A280409_hash(uint64_t k, size_t mask) {
    return (size_t)((k * 0x9E3779B97F4A7C15ULL) >> 20) & mask;
}

static void A280409_grow(void) {
    size_t ncap = A280409_tcap ? A280409_tcap * 2 : 64;
    uint64_t *nt = mem_new(uint64_t, ncap);
    for (size_t i = 0; i < A280409_tcap; i++) {
        uint64_t k = A280409_tab[i];
        if (k) {
            size_t h = A280409_hash(k, ncap - 1);
            while (nt[h]) h = (h + 1) & (ncap - 1);
            nt[h] = k;
        }
    }
    if (A280409_tab) mem_free(A280409_tab);
    A280409_tab = nt;
    A280409_tcap = ncap;
}

/* returns true if newly inserted */
static bool A280409_insert(uint64_t k) {
    if ((A280409_tcnt + 1) * 2 > A280409_tcap) A280409_grow();
    size_t mask = A280409_tcap - 1;
    size_t h = A280409_hash(k, mask);
    while (A280409_tab[h]) {
        if (A280409_tab[h] == k) return false;
        h = (h + 1) & mask;
    }
    A280409_tab[h] = k;
    A280409_tcnt++;
    return true;
}

static void A280409_add(uint64_t p) {
    if (!A280409_insert(p)) return;
    if (A280409_len == A280409_cap) {
        size_t nc = A280409_cap ? A280409_cap * 2 : 64;
        A280409_list = mem_resize(uint64_t, A280409_list, nc);
        A280409_cap = nc;
    }
    A280409_list[A280409_len++] = p;
}

static void A280409_row(uint64_t n) {
    uint64_t m = n;
    while (true) {
        if (is_prime(m)) A280409_add(m);
        if (m == 1) break;
        if ((m & 1ULL) == 0) m >>= 1;
        else m = (uint64_t)((unsigned __int128)3 * m + 1);
    }
}

seq_t a(idx_t n) {
    while (A280409_len < (size_t)n) {
        A280409_row(A280409_next);
        A280409_next++;
    }
    return (seq_t)A280409_list[n - 1];
}
