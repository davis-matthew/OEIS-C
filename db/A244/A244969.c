/*
 * A244969  Duplicate of A090196.
 * https://oeis.org/A244969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th odd integer k having divisors a<b (both >1) with
 *            b<=2a; found by search (duplicate of A090196)
 * Range:     i8 n <= 10, i16 n <= 41, i32 n <= 41, i64 n <= 41, i128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool A244969_has_property(uint64_t k)
{
    if (k < 3) return false;
    uint64_t divs[1024];
    size_t dcnt = 0;
    for (uint64_t d = 2; d * d <= k; ++d) {
        if (k % d == 0) {
            if (dcnt + 2 >= 1024) break;
            divs[dcnt++] = d;
            uint64_t e = k / d;
            if (e != d) divs[dcnt++] = e;
        }
    }
    divs[dcnt++] = k;
    for (size_t i = 0; i < dcnt; ++i)
        for (size_t j = 0; j < dcnt; ++j)
            if (divs[i] < divs[j] && divs[j] <= 2 * divs[i])
                return true;
    return false;
}

static uint64_t *A244969_cache = NULL;
static size_t A244969_len = 0, A244969_cap = 0;
static uint64_t A244969_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A244969_len < (size_t)n) {
        if (A244969_len == A244969_cap) {
            size_t nc = A244969_cap ? 2 * A244969_cap : 64;
            uint64_t *t = (uint64_t *)realloc(A244969_cache, nc * sizeof(uint64_t));
            if (!t) abort();
            A244969_cache = t;
            A244969_cap = nc;
        }
        do {
            A244969_next += 2;
        } while (!A244969_has_property(A244969_next));
        A244969_cache[A244969_len++] = A244969_next;
    }
    return (seq_t)A244969_cache[(size_t)n - 1];
}
