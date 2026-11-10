/*
 * A361346  Numbers k such that A361338(k) = 7.
 * https://oeis.org/A361346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that A361338(k) = 7 (A361338(k) = number of
 *            distinct single digits reachable by split-and-multiply); found
 *            by incremental search with memoised reachable-digit masks
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint16_t *A361346_mask = NULL;
static uint64_t *A361346_found = NULL;
static size_t A361346_nfound = 0, A361346_capf = 0, A361346_capm = 0;
static uint64_t A361346_next = 0; /* next k to examine */

static void A361346_compute(uint64_t v)
{
    if (v >= A361346_capm) {
        size_t nc = A361346_capm ? A361346_capm * 2 : 1024;
        A361346_mask = (uint16_t *)realloc(A361346_mask, nc * sizeof(uint16_t));
        if (!A361346_mask) exit(1);
        A361346_capm = nc;
    }
    if (v <= 9) {
        A361346_mask[v] = (uint16_t)(1u << v);
        return;
    }
    uint16_t m = 0;
    uint64_t p10 = 10, lo = 1; /* p10 = 10^lenR, lo = 10^(lenR-1) */
    int lenR = 1;
    while (p10 <= v) {
        uint64_t left = v / p10, right = v % p10;
        if (!(lenR > 1 && right < lo)) {
            m |= A361346_mask[left * right];
        }
        lo = p10;
        p10 *= 10;
        lenR++;
    }
    A361346_mask[v] = m;
}

seq_t a(idx_t n)
{
    if (A361346_next == 0) {
        A361346_compute(0);
        A361346_next = 1;
    }
    while (A361346_nfound < (size_t)n) {
        uint64_t k = A361346_next++;
        A361346_compute(k);
        uint16_t m = A361346_mask[k];
        int c = 0;
        while (m) { c += m & 1; m >>= 1; }
        if (c == 7) {
            if (A361346_nfound >= A361346_capf) {
                size_t nc = A361346_capf ? A361346_capf * 2 : 256;
                A361346_found = (uint64_t *)realloc(A361346_found, nc * sizeof(uint64_t));
                if (!A361346_found) exit(1);
                A361346_capf = nc;
            }
            A361346_found[A361346_nfound++] = k;
        }
    }
    return (seq_t)A361346_found[n - 1];
}
