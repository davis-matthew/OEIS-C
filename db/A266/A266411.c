/*
 * A266411  a(1) = 1, after which each a(n) = (A004074(n)+1)-th number
 *          selected from those not yet in the sequence.
 * https://oeis.org/A266411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1; a(n) = (A004074(n)+1)-th number not yet in the
 *            sequence, where A004074(n) = 2*A004001(n) - n; computed by
 *            direct simulation with a used-flags array
 * Range:     u8 n <= 248, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 102 terms through n = 7865 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A266411_h = NULL;
static idx_t A266411_hlen = 0;
static idx_t A266411_hcap = 0;

static void A266411_ensure_h(idx_t n) {
    if (n < 2) n = 2;
    if (n <= A266411_hlen) return;
    if (n > A266411_hcap) {
        idx_t nc = (n < 1024) ? 1024 : n + n / 2;
        uint64_t *p = (uint64_t *)realloc(A266411_h, (size_t)nc * sizeof(uint64_t));
        if (p == NULL) abort();
        A266411_h = p;
        A266411_hcap = nc;
    }
    if (A266411_hlen == 0) {
        A266411_h[0] = 1;
        A266411_h[1] = 1;
        A266411_hlen = 2;
    }
    for (idx_t i = A266411_hlen + 1; i <= n; i++) {
        uint64_t prev = A266411_h[i - 2];
        A266411_h[i - 1] = A266411_h[prev - 1] + A266411_h[(i - prev) - 1];
    }
    A266411_hlen = n;
}

static uint64_t *A266411_terms = NULL;
static unsigned char *A266411_used = NULL;
static idx_t A266411_cnt = 0;
static idx_t A266411_cap = 0;
static idx_t A266411_ucap = 0;
static uint64_t A266411_first = 2;

static void A266411_extend(idx_t n) {
    if (n <= A266411_cnt) return;
    A266411_ensure_h(n);
    if (n > A266411_cap) {
        idx_t nc = n + n / 2 + 16;
        uint64_t *p = (uint64_t *)realloc(A266411_terms, (size_t)nc * sizeof(uint64_t));
        if (p == NULL) abort();
        A266411_terms = p;
        A266411_cap = nc;
    }
    idx_t need = 2 * n + 64;
    if (need > A266411_ucap) {
        idx_t nc = need + need / 2;
        unsigned char *p = (unsigned char *)realloc(A266411_used, (size_t)nc);
        if (p == NULL) abort();
        for (idx_t j = A266411_ucap; j < nc; j++) p[j] = 0;
        A266411_used = p;
        A266411_ucap = nc;
    }
    if (A266411_cnt == 0) {
        A266411_terms[0] = 1;
        A266411_used[1] = 1;
        A266411_first = 2;
        A266411_cnt = 1;
    }
    for (idx_t i = A266411_cnt + 1; i <= n; i++) {
        uint64_t d = 2 * A266411_h[i - 1] - (uint64_t)i;
        uint64_t p = A266411_first;
        uint64_t c = 0;
        for (;; p++) {
            if (!A266411_used[p]) {
                if (c == d) break;
                c++;
            }
        }
        A266411_used[p] = 1;
        A266411_terms[i - 1] = p;
        while (A266411_used[A266411_first]) A266411_first++;
    }
    A266411_cnt = n;
}

seq_t a(idx_t n) {
    A266411_extend(n);
    return (seq_t)A266411_terms[n - 1];
}
