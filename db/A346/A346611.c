/*
 * A346611  a(n) = (A203069(n)-n)/2.
 * https://oeis.org/A346611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (A203069(n)-n)/2, where A203069 is built by a greedy
 *            search (smallest unused number of opposite parity with composite
 *            sum)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9700 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A203069_terms = NULL;
static size_t A203069_len = 0, A203069_cap = 0;
static unsigned char *A203069_used = NULL;
static size_t A203069_used_cap = 0;

static void A203069_ensure_used(size_t idx)
{
    if (idx < A203069_used_cap) return;
    size_t nc = A203069_used_cap ? A203069_used_cap * 2 : 1024;
    while (nc <= idx) nc *= 2;
    unsigned char *p = (unsigned char *)realloc(A203069_used, nc);
    if (!p) abort();
    memset(p + A203069_used_cap, 0, nc - A203069_used_cap);
    A203069_used = p;
    A203069_used_cap = nc;
}

static void A203069_generate(size_t n)
{
    if (A203069_cap < n) {
        size_t nc = A203069_cap ? A203069_cap * 2 : 1024;
        while (nc < n) nc *= 2;
        uint64_t *p = (uint64_t *)realloc(A203069_terms, nc * sizeof(uint64_t));
        if (!p) abort();
        A203069_terms = p;
        A203069_cap = nc;
    }
    while (A203069_len < n) {
        if (A203069_len == 0) {
            A203069_ensure_used(1);
            A203069_terms[0] = 1;
            A203069_used[1] = 1;
            A203069_len = 1;
            continue;
        }
        uint64_t prev = A203069_terms[A203069_len - 1];
        size_t cand = (prev & 1) ? 2 : 1;
        for (;;) {
            A203069_ensure_used(cand);
            if (!A203069_used[cand] && !is_prime_u64(prev + (uint64_t)cand)) {
                A203069_terms[A203069_len++] = (uint64_t)cand;
                A203069_used[cand] = 1;
                break;
            }
            cand += 2;
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    A203069_generate((size_t)n);
    uint64_t v = A203069_terms[(size_t)(n - 1)];
    return ((seq_t)v - (seq_t)n) / 2;
}
