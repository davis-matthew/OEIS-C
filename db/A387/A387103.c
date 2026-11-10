/*
 * A387103  For any n >= 2, a(n) is the number of positive values k <
 *          A386482(n-1) missing from the first n-1 terms of A386482 such that
 *          gcd(k, A386482(n-1)) != 1.
 * https://oeis.org/A387103
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{k < A386482(n-1) : k unused among A386482(1..n-1),
 *            gcd(k, A386482(n-1)) > 1}; computed by simulating A386482 with a
 *            list of unused values
 * Range:     u8 n <= 1533, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 99 terms through n = 9964 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static size_t *a387103_terms = NULL;
static size_t *a387103_cnt = NULL;   /* cnt[n] = a(n) */
static size_t a387103_len = 0, a387103_cap = 0;
static size_t *a387103_holes = NULL; /* sorted unused values below maxv */
static size_t a387103_hn = 0, a387103_hcap = 0;
static size_t a387103_maxv = 0;

static size_t a387103_gcd(size_t x, size_t y)
{
    while (y) { size_t t = x % y; x = y; y = t; }
    return x;
}

static void a387103_push_hole(size_t v)
{
    if (a387103_hn == a387103_hcap) {
        size_t nc = a387103_hcap ? a387103_hcap * 2 : 64;
        size_t *t = (size_t *)realloc(a387103_holes, nc * sizeof(size_t));
        if (!t) abort();
        a387103_holes = t;
        a387103_hcap = nc;
    }
    a387103_holes[a387103_hn++] = v;
}

static void a387103_push_term(size_t v, size_t c)
{
    if (a387103_len + 2 >= a387103_cap) {
        size_t nc = a387103_cap ? a387103_cap * 2 : 64;
        size_t *t = (size_t *)realloc(a387103_terms, nc * sizeof(size_t));
        size_t *u = NULL;
        if (!t) abort();
        a387103_terms = t;
        u = (size_t *)realloc(a387103_cnt, nc * sizeof(size_t));
        if (!u) abort();
        a387103_cnt = u;
        a387103_cap = nc;
    }
    a387103_terms[a387103_len] = v;
    a387103_len++;
    a387103_cnt[a387103_len] = c; /* index n = len */
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    if (a387103_len == 0) {
        a387103_push_term(1, 0);
        a387103_push_term(2, 0);
        a387103_maxv = 2;
    }
    while (a387103_len < (size_t)n) {
        size_t prev = a387103_terms[a387103_len - 1];
        size_t count = 0;
        bool have = false;
        size_t best = 0, i = 0, chosen = 0;
        for (i = 0; i < a387103_hn; i++) {
            size_t h = a387103_holes[i];
            if (h >= prev) break;
            if (a387103_gcd(prev, h) > 1) { count++; best = i; have = true; }
        }
        if (have) {
            chosen = a387103_holes[best];
            memmove(a387103_holes + best, a387103_holes + best + 1,
                    (a387103_hn - best - 1) * sizeof(size_t));
            a387103_hn--;
        } else {
            bool found = false;
            for (; i < a387103_hn; i++) {
                if (a387103_gcd(prev, a387103_holes[i]) > 1) {
                    chosen = a387103_holes[i];
                    memmove(a387103_holes + i, a387103_holes + i + 1,
                            (a387103_hn - i - 1) * sizeof(size_t));
                    a387103_hn--;
                    found = true;
                    break;
                }
            }
            if (!found) {
                size_t v = a387103_maxv + 1;
                for (;; v++) {
                    if (a387103_gcd(prev, v) > 1) break;
                    a387103_push_hole(v);
                }
                chosen = v;
                a387103_maxv = v;
            }
        }
        a387103_push_term(chosen, count);
    }
    return (seq_t)a387103_cnt[(size_t)n];
}
