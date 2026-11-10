/*
 * A331005  a(n) is the imaginary part of f(n) (as defined in A330780).
 * https://oeis.org/A331005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Im(f(n)), f(n) = Sum_{k=1..n} A330780(k)*m(k), with m(k)
 *            the 8-periodic knight moves (2+i, 1+2i, -1+2i, -2+i, -2-i,
 *            -1-2i, 1-2i, 2-i); A330780 is built by a greedy search with a
 *            hash set for injectivity
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9797 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static const int64_t A331005_mre[8] = {  2,  1, -1, -2, -2, -1,  1,  2 };
static const int64_t A331005_mim[8] = {  1,  2,  2,  1, -1, -2, -2, -1 };

static size_t A331005_K = 0;          /* terms computed; sums 0..K stored */
static int64_t *A331005_sr = NULL;
static int64_t *A331005_si = NULL;
static size_t A331005_cap = 0;
static size_t *A331005_tab = NULL;
static size_t A331005_tabsz = 0;
static size_t *A331005_cnt = NULL;
static size_t A331005_cntcap = 0;

static size_t A331005_hash(int64_t r, int64_t i)
{
    uint64_t h = (uint64_t)r * 0x9E3779B97F4A7C15ULL
               ^ ((uint64_t)i + 0x7F4A7C15ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    h *= 0xBF58476D1CE4E5B9ULL;
    h ^= h >> 32;
    return (size_t)h;
}

static bool A331005_has(int64_t r, int64_t i)
{
    size_t mask = A331005_tabsz - 1;
    size_t p = A331005_hash(r, i) & mask;
    while (A331005_tab[p]) {
        size_t j = A331005_tab[p] - 1;
        if (A331005_sr[j] == r && A331005_si[j] == i) return true;
        p = (p + 1) & mask;
    }
    return false;
}

static void A331005_ins(size_t j)
{
    size_t mask = A331005_tabsz - 1;
    size_t p = A331005_hash(A331005_sr[j], A331005_si[j]) & mask;
    while (A331005_tab[p]) p = (p + 1) & mask;
    A331005_tab[p] = j + 1;
}

static void A331005_grow(void)
{
    /* ensure room for one more sum (index K+1) */
    if (A331005_K + 2 > A331005_cap) {
        size_t nc = A331005_cap ? A331005_cap * 2 : 1024;
        A331005_sr = (int64_t *)realloc(A331005_sr, nc * sizeof(int64_t));
        A331005_si = (int64_t *)realloc(A331005_si, nc * sizeof(int64_t));
        A331005_cap = nc;
    }
    if ((A331005_K + 2) * 2 > A331005_tabsz) {
        size_t ns = A331005_tabsz ? A331005_tabsz * 2 : 4096;
        free(A331005_tab);
        A331005_tab = (size_t *)calloc(ns, sizeof(size_t));
        A331005_tabsz = ns;
        for (size_t j = 0; j <= A331005_K; j++) A331005_ins(j);
    }
}

seq_t a(idx_t n)
{
    if (A331005_cap == 0) {
        A331005_grow();
        A331005_sr[0] = 0;
        A331005_si[0] = 0;
        A331005_ins(0);
        A331005_cntcap = 256;
        A331005_cnt = (size_t *)calloc(A331005_cntcap, sizeof(size_t));
    }
    while (A331005_K < (size_t)n) {
        A331005_grow();
        size_t pos = A331005_K % 8;
        int64_t cr = A331005_sr[A331005_K];
        int64_t ci = A331005_si[A331005_K];
        for (size_t v = 1; ; v++) {
            if (v >= A331005_cntcap) {
                size_t nc = A331005_cntcap * 2;
                A331005_cnt = (size_t *)realloc(A331005_cnt, nc * sizeof(size_t));
                memset(A331005_cnt + A331005_cntcap, 0,
                       (nc - A331005_cntcap) * sizeof(size_t));
                A331005_cntcap = nc;
            }
            if (A331005_cnt[v] >= v * v) continue;
            int64_t nr = cr + (int64_t)v * A331005_mre[pos];
            int64_t ni = ci + (int64_t)v * A331005_mim[pos];
            if (A331005_has(nr, ni)) continue;
            A331005_cnt[v]++;
            A331005_sr[A331005_K + 1] = nr;
            A331005_si[A331005_K + 1] = ni;
            A331005_K++;
            A331005_ins(A331005_K);
            break;
        }
    }
    return (seq_t)A331005_si[n];
}
