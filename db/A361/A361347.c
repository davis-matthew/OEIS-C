/*
 * A361347  Numbers k such that A361338(k) = 8.
 * https://oeis.org/A361347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that A361338(k) = 8, found by a search over k
 *            with memoized split-and-multiply reachable-digit masks (A361338
 *            logic)
 * Range:     u8 none, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Validated: partial, pass, 52 terms through n = 52 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A361347_LIM (1u << 22)

static uint16_t *A361347_cache = NULL;
static uint64_t *A361347_terms = NULL;
static size_t A361347_cnt = 0, A361347_cap = 0;
static uint64_t A361347_next = 1;

static unsigned A361347_mask(uint64_t v)
{
    if (v <= 9) return 1u << (unsigned)v;
    if (v < A361347_LIM && (A361347_cache[v] & 0x8000u))
        return A361347_cache[v] & 0x7FFFu;

    unsigned m = 0;
    uint64_t pw = 10, t = v;
    int len = 0;
    while (t) { len++; t /= 10; }
    for (int lenR = 1; lenR < len; ++lenR) {
        /* pw = 10^lenR */
        uint64_t left = v / pw, right = v % pw;
        if (!(lenR > 1 && right < pw / 10))
            m |= A361347_mask(left * right);
        pw *= 10;
    }
    if (v < A361347_LIM) A361347_cache[v] = (uint16_t)(m | 0x8000u);
    return m;
}

seq_t a(idx_t n)
{
    if (!A361347_cache)
        A361347_cache = (uint16_t *)calloc(A361347_LIM, sizeof(uint16_t));
    while (A361347_cnt < (size_t)n) {
        uint64_t k = A361347_next++;
        unsigned m = A361347_mask(k);
        int c = 0;
        while (m) { c += m & 1u; m >>= 1; }
        if (c == 8) {
            if (A361347_cnt == A361347_cap) {
                A361347_cap = A361347_cap ? A361347_cap * 2 : 256;
                A361347_terms = (uint64_t *)realloc(A361347_terms, A361347_cap * sizeof(uint64_t));
            }
            A361347_terms[A361347_cnt++] = k;
        }
    }
    return (seq_t)A361347_terms[(size_t)n - 1];
}
