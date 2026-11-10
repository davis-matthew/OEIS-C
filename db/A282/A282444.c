/*
 * A282444  Numbers k such that A282442(k) = k + 1.
 * https://oeis.org/A282444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search over k; for each k, the staircase walk is run backward
 *            from its forced final state, (position 0 after step
 *            2*floor(k/2)), down to the start, in O(1) batches where the
 *            quotient is constant. a(n) is the n-th k for which every step
 *            size 1..k is taken, i.e. A282442(k) = k + 1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 19 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *A282444_terms = NULL;
static size_t A282444_cnt = 0, A282444_cap = 0;
static uint64_t A282444_next = 1;

/* State (i, p): p = position after even step 2i (0 <= p < 2i).
   Backward step to (i-1, p'):  K = floor((m-p)/(2i)), D = (m-p) mod 2i,
   need K >= 1, D < 2i-1, and p' = (p+K) mod (2i-1) < 2i-2;
   at i == 1 need D == 0 (step 1 reaches the top from position 0). */
static bool A282444_ok(uint64_t m) {
    uint64_t i = m / 2, p = 0;
    if (i == 0) return true;
    for (;;) {
        uint64_t t = m - p;
        uint64_t K = t / (2 * i);
        uint64_t D = t - K * 2 * i;
        if (K == 0) return false;
        if (i == 1) return D == 0;
        if (D >= 2 * i - 1) return false;
        if (2 * i >= 2 + D && 2 * i >= 3 + p + K) {
            uint64_t c = K + 2;
            uint64_t T1 = (2 * i - 2 - D) / c;
            uint64_t T2 = (2 * i - 3 - p - K) / c;
            uint64_t T3 = i - 2;
            uint64_t T = T1;
            if (T2 < T) T = T2;
            if (T3 < T) T = T3;
            T += 1;
            i -= T;
            p += T * K;
            continue;
        }
        {
            uint64_t q = (p + K) % (2 * i - 1);
            if (q >= 2 * i - 2) return false;
            p = q;
            i -= 1;
        }
    }
}

seq_t a(idx_t n) {
    while (A282444_cnt < n) {
        uint64_t m = A282444_next++;
        if (A282444_ok(m)) {
            if (A282444_cnt == A282444_cap) {
                size_t nc = A282444_cap ? A282444_cap * 2 : 64;
                uint64_t *nt = (uint64_t *)realloc(A282444_terms, nc * sizeof(uint64_t));
                if (!nt) return (seq_t)0;
                A282444_terms = nt;
                A282444_cap = nc;
            }
            A282444_terms[A282444_cnt++] = m;
        }
    }
    return (seq_t)A282444_terms[n - 1];
}
