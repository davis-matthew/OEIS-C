/*
 * A379166  Numbers in A379165 that share a factor with both previous terms,
 *          in order of appearance.
 * https://oeis.org/A379166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search/simulation. Generate A379165 (a(1)=1, a(2)=2;
 *            a(n)=smallest unused k coprime to both a(n-1),a(n-2) or sharing
 *            a factor with both); A379166 lists, in order of appearance, the
 *            terms that share a factor with both previous terms
 * Range:     u8 n <= 76, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9858 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static bool *A379166_used = NULL;
static size_t A379166_used_cap = 0;
static uint64_t *A379166_found = NULL;
static size_t A379166_found_cnt = 0, A379166_found_cap = 0;
static uint64_t A379166_p1 = 0, A379166_p2 = 0, A379166_low = 1;
static bool A379166_init = false;

static uint64_t A379166_gcd(uint64_t x, uint64_t y) {
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static void A379166_grow_used(size_t need) {
    if (need < A379166_used_cap)
        return;
    size_t nc = A379166_used_cap ? A379166_used_cap : 1024;
    while (nc <= need)
        nc *= 2;
    bool *p = (bool *)realloc(A379166_used, nc * sizeof(bool));
    if (!p)
        abort();
    for (size_t i = A379166_used_cap; i < nc; ++i)
        p[i] = false;
    A379166_used = p;
    A379166_used_cap = nc;
}

static void A379166_push(uint64_t v) {
    if (A379166_found_cnt == A379166_found_cap) {
        size_t nc = A379166_found_cap ? A379166_found_cap * 2 : 256;
        uint64_t *p = (uint64_t *)realloc(A379166_found, nc * sizeof(uint64_t));
        if (!p)
            abort();
        A379166_found = p;
        A379166_found_cap = nc;
    }
    A379166_found[A379166_found_cnt++] = v;
}

seq_t a(idx_t n) {
    if (!A379166_init) {
        A379166_init = true;
        A379166_grow_used(1024);
        A379166_used[1] = true;
        A379166_used[2] = true;
        A379166_p2 = 1;
        A379166_p1 = 2;
        A379166_low = 3;
    }
    while (A379166_found_cnt < (size_t)n) {
        uint64_t cand;
        bool share = false;
        for (cand = A379166_low; ; ++cand) {
            A379166_grow_used((size_t)cand + 1);
            if (A379166_used[cand])
                continue;
            uint64_t g1 = A379166_gcd(A379166_p1, cand);
            uint64_t g0 = A379166_gcd(A379166_p2, cand);
            if (g1 == 1 && g0 == 1) {
                share = false;
                break;
            }
            if (g1 > 1 && g0 > 1) {
                share = true;
                break;
            }
        }
        A379166_used[cand] = true;
        while (A379166_used[A379166_low])
            ++A379166_low;
        if (share)
            A379166_push(cand);
        A379166_p2 = A379166_p1;
        A379166_p1 = cand;
        A379166_grow_used((size_t)A379166_low + 1);
    }
    return (seq_t)A379166_found[(size_t)n - 1];
}
