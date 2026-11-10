/*
 * A324792  First differences of A325056: distance in A076042 from n-th low
 *          point to the next.
 * https://oeis.org/A324792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A325056(n+1) - A325056(n), where A325056 lists the low
 *            points of A076042: index 0, and every j >= 3 with A076042(j) <
 *            2j+3. A076042 is not stepped one term at a time. From a low
 *            point L with value v, the next values are v+(L+1)^2, then
 *            v+(L+1)^2+(L+2)^2, then w = v+L^2-4 at index L+3. Afte...
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 41 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef unsigned __int128 A324792_u128;

static uint64_t *A324792_lows = NULL;
static size_t A324792_nlows = 0, A324792_cap = 0;
static uint64_t A324792_L = 0;      /* last low point found */
static A324792_u128 A324792_v = 0;  /* A076042 value at that low point */

static void A324792_push(uint64_t x) {
    if (A324792_nlows == A324792_cap) {
        size_t nc = A324792_cap ? A324792_cap * 2 : 64;
        uint64_t *t = (uint64_t *)realloc(A324792_lows, nc * sizeof(uint64_t));
        if (!t) exit(1);
        A324792_lows = t;
        A324792_cap = nc;
    }
    A324792_lows[A324792_nlows++] = x;
}

/* true iff the alternating chain starting at index j0 with value w
   hits a low point at step k */
static int A324792_cond(A324792_u128 w, A324792_u128 j0, A324792_u128 k) {
    A324792_u128 rhs = 2 * k * k + k * (2 * j0 + 5) + 2 * j0 + 3;
    return w < rhs;
}

static void A324792_next(void) {
    A324792_u128 L = A324792_L;
    A324792_u128 j0 = L + 3;
    A324792_u128 w = A324792_v + L * L - 4;
    A324792_u128 lo, hi = 1;
    if (A324792_cond(w, j0, 0)) {
        hi = 0;
    } else {
        lo = 0;
        while (!A324792_cond(w, j0, hi)) { lo = hi; hi *= 2; }
        /* cond(lo) false, cond(hi) true */
        while (hi - lo > 1) {
            A324792_u128 mid = lo + (hi - lo) / 2;
            if (A324792_cond(w, j0, mid)) hi = mid; else lo = mid;
        }
    }
    A324792_u128 k = hi;
    A324792_u128 S = 2 * k * k + k * (2 * j0 + 1);
    A324792_L = (uint64_t)(j0 + 2 * k);
    A324792_v = w - S;
    A324792_push(A324792_L);
}

seq_t a(idx_t n) {
    if (A324792_nlows == 0) {
        A324792_push(0);
        A324792_push(5);      /* A076042(5) = 5 */
        A324792_L = 5;
        A324792_v = 5;
    }
    while (A324792_nlows < (size_t)n + 2) A324792_next();
    return (seq_t)(A324792_lows[n + 1] - A324792_lows[n]);
}
