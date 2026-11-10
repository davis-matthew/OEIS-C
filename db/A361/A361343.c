/*
 * A361343  Numbers k such that A361338(k) = 4.
 * https://oeis.org/A361343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that A361338(k) = 4; sieve-style search
 *            computing reachable single-digit masks incrementally
 * Range:     u8 n <= 1, u16 n <= 1020, u32 n <= 1037, u64 n <= 1037,
 *            u128 n <= 1037, big any n
 * Validated: standard, pass, 86 terms through n = 1036 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint16_t *A361343_mask = NULL;
static uint64_t *A361343_terms = NULL;
static uint64_t A361343_nm = 0, A361343_capm = 0;
static uint64_t A361343_nt = 0, A361343_capt = 0;

static void A361343_extend_one(void)
{
    uint64_t v = A361343_nm;
    if (v >= A361343_capm) {
        uint64_t nc = A361343_capm ? A361343_capm * 2 : 1024;
        A361343_mask = (uint16_t *)realloc(A361343_mask, nc * sizeof(uint16_t));
        if (!A361343_mask) exit(1);
        A361343_capm = nc;
    }
    unsigned m = 0;
    if (v <= 9) {
        m = 1u << (unsigned)v;
    } else {
        uint64_t div = 10, lo = 1;
        while (div <= v / 10 + 0 && div * 10 <= v) { div *= 10; }
        /* iterate over all split points: div = 10^lenR for lenR = 1..len-1 */
        uint64_t d = 10;
        uint64_t prev = 1;
        while (d <= v) {
            uint64_t left = v / d, right = v % d;
            if (!(prev > 1 && right < prev)) {
                m |= A361343_mask[left * right];
            }
            prev = d;
            d *= 10;
        }
        (void)lo;
        (void)div;
    }
    A361343_mask[v] = (uint16_t)m;
    A361343_nm++;
    if (v >= 1 && __builtin_popcount(m) == 4) {
        if (A361343_nt >= A361343_capt) {
            uint64_t nc = A361343_capt ? A361343_capt * 2 : 256;
            A361343_terms = (uint64_t *)realloc(A361343_terms, nc * sizeof(uint64_t));
            if (!A361343_terms) exit(1);
            A361343_capt = nc;
        }
        A361343_terms[A361343_nt++] = v;
    }
}

seq_t a(idx_t n)
{
    while (A361343_nt < (uint64_t)n)
        A361343_extend_one();
    return (seq_t)A361343_terms[n - 1];
}
