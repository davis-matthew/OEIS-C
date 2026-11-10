/*
 * A358403  The index of A358402 where n first appears, or 0 if n never
 *          appears.
 * https://oeis.org/A358403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least index k such that A358402(k) = n, or 0 if none
 *            appears (found by generating A358402 with first/last-occurrence
 *            tables up to a fixed limit); A358402(1)=0, A358402(k)=min(k-1-p,
 *            f) where m=A358402(k-1), p its previous occurrence, f its first
 *            occurrence, or 0 if m is new
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 61 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define A358403_LIMIT 3000000u

static uint32_t *A358403_first = NULL;
static uint32_t *A358403_last = NULL;
static uint32_t A358403_cur = 0;
static uint32_t A358403_curval = 0;
static uint32_t A358403_curprev = 0;

static void A358403_extend(idx_t n) {
    if (!A358403_first) {
        A358403_first = (uint32_t *)calloc((size_t)A358403_LIMIT + 2, sizeof(uint32_t));
        A358403_last = (uint32_t *)calloc((size_t)A358403_LIMIT + 2, sizeof(uint32_t));
        if (!A358403_first || !A358403_last) exit(1);
        A358403_cur = 1;
        A358403_curval = 0;
        A358403_curprev = 0;
        A358403_first[0] = 1;
        A358403_last[0] = 1;
    }
    while (A358403_cur < A358403_LIMIT && A358403_first[n] == 0) {
        uint32_t m = A358403_curval;
        uint32_t val;
        if (A358403_curprev == 0) {
            val = 0;
        } else {
            uint32_t dist = A358403_cur - A358403_curprev;
            uint32_t f = A358403_first[m];
            val = dist < f ? dist : f;
        }
        A358403_cur++;
        A358403_curprev = A358403_last[val];
        A358403_last[val] = A358403_cur;
        if (A358403_first[val] == 0) A358403_first[val] = A358403_cur;
        A358403_curval = val;
    }
}

seq_t a(idx_t n) {
    if (n >= (idx_t)A358403_LIMIT) return 0;
    if (!A358403_first || A358403_first[n] == 0) A358403_extend(n);
    return (seq_t)A358403_first[n];
}
