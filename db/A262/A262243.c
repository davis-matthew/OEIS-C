/*
 * A262243  First differences of A262224.
 * https://oeis.org/A262243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A262224(n+1) - A262224(n) = A047813(A262224(n)), with
 *            A262224(0) = 1 and A262224(k+1) = A262224(k) +
 *            A047813(A262224(k))
 * Range:     u8 n <= 10, u16 n <= 193, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9908 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Largest palindromic substring (no leading zero) of the decimal digits of v. */
static seq_t A262243_lps(seq_t v)
{
    size_t cap = 64, len = 0;
    char *d = (char *)malloc(cap);
    char *r;
    seq_t t = v;
    while (t > 0) {
        if (len == cap) {
            cap *= 2;
            d = (char *)realloc(d, cap);
        }
        d[len++] = (char)(t % 10);
        t /= 10;
    }
    /* d is in reverse order; reverse to forward. */
    for (size_t i = 0; i < len / 2; ++i) {
        char tmp = d[i];
        d[i] = d[len - 1 - i];
        d[len - 1 - i] = tmp;
    }
    size_t bestLen = 0, bestStart = 0;
    for (size_t c = 0; c < 2 * len; ++c) {
        long long lo = (long long)(c / 2);
        long long hi = lo + (long long)(c % 2);
        while (lo >= 0 && hi < (long long)len && d[lo] == d[hi]) {
            if (d[lo] != 0) {
                size_t L = (size_t)(hi - lo + 1);
                if (L > bestLen ||
                    (L == bestLen && memcmp(d + lo, d + bestStart, L) > 0)) {
                    bestLen = L;
                    bestStart = (size_t)lo;
                }
            }
            --lo;
            ++hi;
        }
    }
    seq_t val = 0;
    for (size_t k = 0; k < bestLen; ++k)
        val = val * 10 + (seq_t)(int)d[bestStart + k];
    r = d;
    free(r);
    return val;
}

static seq_t *A262243_diffs = 0;
static size_t A262243_cnt = 0, A262243_cap = 0;
static seq_t *A262243_cur = 0;

seq_t a(idx_t n)
{
    if (!A262243_cur) {
        A262243_cur = mem_new(seq_t, 1);
        A262243_cur[0] = 1;
    }
    while (A262243_cnt <= (size_t)n) {
        if (A262243_cnt == A262243_cap) {
            size_t nc = A262243_cap ? A262243_cap * 2 : 1024;
            if (!A262243_diffs)
                A262243_diffs = mem_new(seq_t, nc);
            else
                A262243_diffs = mem_resize(seq_t, A262243_diffs, nc);
            A262243_cap = nc;
        }
        seq_t p = A262243_lps(A262243_cur[0]);
        A262243_diffs[A262243_cnt++] = p;
        A262243_cur[0] = A262243_cur[0] + p;
    }
    return A262243_diffs[(size_t)n];
}
