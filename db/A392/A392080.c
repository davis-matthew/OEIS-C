/*
 * A392080  a(n) is the number of repeated difference values among the
 *          consecutive divisors of A259366(n).
 * https://oeis.org/A392080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values occurring more than once among
 *            the differences of consecutive divisors of A259366(n); A259366
 *            found by incremental search
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9921 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static int A392080_cmp(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* Returns number of repeated gap values of m (0 if none). */
static int A392080_repeats(uint64_t m)
{
    if (m < 4) return 0;
    uint64_t divs[4096];
    uint64_t gaps[4096];
    int dcnt = 0;
    for (uint64_t d = 1; d <= m / d; ++d) {
        if (m % d == 0) {
            divs[dcnt++] = d;
            if (d != m / d) divs[dcnt++] = m / d;
        }
    }
    if (dcnt <= 2) return 0;
    qsort(divs, dcnt, sizeof(uint64_t), A392080_cmp);
    int g = dcnt - 1;
    for (int i = 1; i < dcnt; ++i) gaps[i - 1] = divs[i] - divs[i - 1];
    qsort(gaps, g, sizeof(uint64_t), A392080_cmp);
    int rep = 0;
    int i = 0;
    while (i < g) {
        int j = i + 1;
        while (j < g && gaps[j] == gaps[i]) ++j;
        if (j - i >= 2) ++rep;
        i = j;
    }
    return rep;
}

static uint64_t *A392080_cache = NULL;
static size_t A392080_len = 0;
static size_t A392080_cap = 0;
static uint64_t A392080_next = 1;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while (A392080_len < (size_t)n) {
        int r = A392080_repeats(A392080_next);
        if (r > 0) {
            if (A392080_len >= A392080_cap) {
                size_t nc = A392080_cap ? A392080_cap * 2 : 64;
                uint64_t *t = mem_resize(uint64_t, A392080_cache, nc);
                if (!t) abort();
                A392080_cache = t;
                A392080_cap = nc;
            }
            A392080_cache[A392080_len++] = (uint64_t)r;
        }
        ++A392080_next;
    }
    return (seq_t)A392080_cache[n - 1];
}
