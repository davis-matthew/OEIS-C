/*
 * A325529  Difference sequence of A325465.
 * https://oeis.org/A325529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A325465(n+1) - A325465(n); A325465 is built by a sieve
 *            (2x and 3x-2 of complement members)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9949 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A325465_terms  = NULL;
static size_t A325465_terms_cap = 0;
static idx_t  A325465_terms_len = 0;

static bool *A325465_marked = NULL;
static idx_t  A325465_marked_cap = 0;

static idx_t A325465_next_check = 3;
static bool A325465_initialized = false;

static void A325465_ensure_terms_capacity(size_t new_cap)
{
    if (new_cap <= A325465_terms_cap) return;
    size_t cap = A325465_terms_cap ? A325465_terms_cap : 64;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A325465_terms, cap);
    if (!tmp) abort();
    A325465_terms = tmp;
    A325465_terms_cap = cap;
}

static void A325465_ensure_marked_capacity(idx_t needed)
{
    if (needed < A325465_marked_cap) return;
    size_t cap = A325465_marked_cap ? A325465_marked_cap : 64;
    while (cap <= (size_t)needed) cap <<= 1;
    bool *tmp = (bool *)realloc(A325465_marked, cap * sizeof(bool));
    if (!tmp) abort();
    for (size_t i = A325465_marked_cap; i < cap; ++i) tmp[i] = false;
    A325465_marked = tmp;
    A325465_marked_cap = (idx_t)cap;
}

static void A325465_ensure_up_to_n(idx_t n)
{
    if (A325465_terms_len >= n) return;

    if (!A325465_initialized) {
        A325465_ensure_marked_capacity(2);
        A325465_marked[0] = false;
        A325465_marked[1] = false;
        A325465_marked[2] = true;

        A325465_ensure_terms_capacity(1);
        A325465_terms[0] = (seq_t)2;
        A325465_terms_len = 1;
        A325465_next_check = 3;
        A325465_initialized = true;
    }

    while (A325465_terms_len < n) {
        A325465_ensure_marked_capacity(A325465_next_check);

        if (!A325465_marked[A325465_next_check]) {
            idx_t v1 = A325465_next_check << 1;
            idx_t v2 = A325465_next_check * 3 - 2;
            A325465_ensure_marked_capacity(v1);
            A325465_ensure_marked_capacity(v2);
            A325465_marked[v1] = true;
            A325465_marked[v2] = true;
        }

        if (A325465_marked[A325465_next_check]) {
            A325465_ensure_terms_capacity((size_t)A325465_terms_len + 1);
            A325465_terms[A325465_terms_len++] = (seq_t)A325465_next_check;
        }
        ++A325465_next_check;
    }
}

static seq_t A325465_a(idx_t n)
{
    A325465_ensure_up_to_n(n);
    return A325465_terms[n - 1];
}

seq_t a(idx_t n)
{
    seq_t hi = A325465_a(n + 1);
    seq_t lo = A325465_a(n);
    return hi - lo;
}
