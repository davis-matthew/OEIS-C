/*
 * A387089  Smallest number missing in the sequence A386482 after n terms.
 * https://oeis.org/A387089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   computed by simulation: generate A386482 term by term (greatest
 *            unused k < a(n-1) with gcd(k,a(n-1)) > 1, else smallest unused k
 *            > a(n-1) with gcd > 1), tracking the smallest positive integer
 *            not yet used; a(n) = that mex after n terms
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9768 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static uint8_t *A387089_used = NULL;
static size_t A387089_used_cap = 0;
static uint64_t *A387089_mex = NULL;   /* mex after k terms stored at index k-1 */
static size_t A387089_len = 0;
static size_t A387089_cap = 0;
static uint64_t A387089_last = 0;
static uint64_t A387089_m = 1;

static uint64_t A387089_gcd(uint64_t x, uint64_t y)
{
    while (y) {
        uint64_t t = x % y;
        x = y;
        y = t;
    }
    return x;
}

static void A387089_ensure_used(size_t need)
{
    if (need < A387089_used_cap) return;
    size_t nc = A387089_used_cap ? A387089_used_cap : 64;
    while (nc <= need) nc <<= 1;
    uint8_t *t = (uint8_t *)realloc(A387089_used, nc);
    if (!t) abort();
    memset(t + A387089_used_cap, 0, nc - A387089_used_cap);
    A387089_used = t;
    A387089_used_cap = nc;
}

static void A387089_push(uint64_t term)
{
    A387089_ensure_used((size_t)term + 1);
    A387089_used[term] = 1;
    A387089_last = term;
    while (1) {
        A387089_ensure_used((size_t)A387089_m + 1);
        if (!A387089_used[A387089_m]) break;
        A387089_m++;
    }
    if (A387089_len >= A387089_cap) {
        size_t nc = A387089_cap ? A387089_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A387089_mex, nc * sizeof(uint64_t));
        if (!t) abort();
        A387089_mex = t;
        A387089_cap = nc;
    }
    A387089_mex[A387089_len++] = A387089_m;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A387089_len == 0) {
        A387089_push(1);
        A387089_push(2);
    }
    while (A387089_len < (size_t)n) {
        uint64_t prev = A387089_last;
        uint64_t cand = 0;
        bool found = false;
        for (uint64_t d = prev; d-- > 2; ) {
            if (!A387089_used[d] && A387089_gcd(prev, d) > 1) {
                cand = d;
                found = true;
                break;
            }
        }
        if (!found) {
            for (uint64_t u = prev + 1;; u++) {
                A387089_ensure_used((size_t)u + 1);
                if (!A387089_used[u] && A387089_gcd(prev, u) > 1) {
                    cand = u;
                    break;
                }
            }
        }
        A387089_push(cand);
    }
    return (seq_t)A387089_mex[(size_t)n - 1];
}
