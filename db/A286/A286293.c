/*
 * A286293  A compressed version of A286290.
 * https://oeis.org/A286293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A286290(n+1) - A286290(n) - 4, where A286290 is generated
 *            by: S = {1}; for m = 2,3,...: if m in S add (m-1)*(m+1); else if
 *            m+1 not in S add m*(m+1); else skip m
 * Range:     u8 n <= 81, u16 n <= 971, u32 n <= 971, u64 n <= 971,
 *            u128 n <= 971, big any n
 * Validated: standard, pass, 80 terms through n = 852 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t *A286293_s = NULL;
static size_t A286293_len = 0;
static size_t A286293_cap = 0;
static uint64_t A286293_next_m = 2;

static bool A286293_contains(uint64_t v)
{
    size_t lo = 0, hi = A286293_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (A286293_s[mid] < v)
            lo = mid + 1;
        else if (A286293_s[mid] > v)
            hi = mid;
        else
            return true;
    }
    return false;
}

static void A286293_push(uint64_t v)
{
    if (A286293_len >= A286293_cap) {
        size_t nc = A286293_cap ? A286293_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A286293_s, nc * sizeof(uint64_t));
        if (!t)
            abort();
        A286293_s = t;
        A286293_cap = nc;
    }
    A286293_s[A286293_len++] = v;
}

/* Ensure at least `count` terms of A286290 are generated. */
static void A286293_fill(size_t count)
{
    if (A286293_len == 0) {
        A286293_push(1);
        A286293_next_m = 2;
    }
    while (A286293_len < count) {
        uint64_t m = A286293_next_m++;
        if (A286293_contains(m))
            A286293_push((m - 1) * (m + 1));
        else if (!A286293_contains(m + 1))
            A286293_push(m * (m + 1));
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    A286293_fill((size_t)n + 1);
    uint64_t d = A286293_s[(size_t)n] - A286293_s[(size_t)(n - 1)];
    return (seq_t)(d - 4);
}
