/*
 * A381662  a(n) = A378684(A380200(n)).
 * https://oeis.org/A381662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is obtained by reading the triangular array defined in OEIS
 *            A381662. The row number r containing position n satisfies 2r²−r
 *            ≥ n and 2(r−1)²−(r−1) < n. Let the cumulative length up to row
 *            r−1 be S_prev. Then k = n−S_prev (1 ≤ k ≤ 4r−3). Set m = 2r−1
 *            and base = (r−1)(2r−3). The offset P is P =...
 * Range:     u8 n <= 253, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   cumulative_sum_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cumulative_sum_uint64.h"

/* Integer square root of a 64‑bit unsigned integer */
static inline uint64_t floor_isqrt_u64(uint64_t x)
{
    uint64_t low = 0, high = x, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        if (mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Main OEIS sequence function */
seq_t a(idx_t n)
{
    /* ------- Find row r ------- */
    uint64_t n64 = (uint64_t)n;
    uint64_t d = 8 * n64 + 1;                /* 1 + 8n */
    uint64_t s = floor_isqrt_u64(d);          /* floor(sqrt(d)) */
    uint64_t r = (1 + s) >> 2;                /* (1 + s) / 4 */

    /* Correct r upward if necessary */
    seq_t S_r = cumulative_sum_uint64(r);
    while (S_r < (seq_t)n64) {
        ++r;
        S_r = cumulative_sum_uint64(r);
    }

    /* ------- Compute k (position in the row) ------- */
    seq_t S_prev = (r == 1) ? (seq_t)0 : cumulative_sum_uint64(r - 1);
    uint64_t k = (uint64_t)((seq_t)n64 - S_prev);  /* 1 ≤ k ≤ 4r−3 */

    /* ------- Compute base and offset P ------- */
    uint64_t m = (2 * r) - 1;
    /* base = (r−1)(2r−3) */
    __uint128_t base128 = (__uint128_t)(r - 1) * (2 * r - 3);
    seq_t base = (seq_t)base128;

    seq_t P;
    if (k < m) {
        if (k & 1u) { /* k odd */
            P = (seq_t)m - (seq_t)k - (seq_t)1;
        } else {      /* k even */
            P = (seq_t)k;
        }
    } else { /* k >= m */
        if (k & 1u) { /* k odd */
            P = (seq_t)k;
        } else {      /* k even */
            P = (seq_t)m * (seq_t)3 - (seq_t)k - (seq_t)1;
        }
    }

    /* ------- Return result ------- */
    return base + P;
}
