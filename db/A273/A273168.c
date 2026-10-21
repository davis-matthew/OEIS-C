/*
 * A273168  Denominators of coefficient triangle for expansion of x^(2*n) in
 *          terms of Chebyshev polynomials of the first kind T(2*m, x)
 *          (A127674).
 * https://oeis.org/A273168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   a(n) = 2^{e(n)} where e(n) is - if m = 0 (first entry of row n):
 *            e = 2n − popcount(n) - if m > 0 (row n, column m): let k = n−m;
 *            e = (2n−1) − (popcount(k)+popcount(n+m)−popcount(2n)) The
 *            sequence is the flattened triangle of these denominators
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

static inline uint64_t popcount_u64(uint64_t x) {
    return __builtin_popcountll(x);
}

/* find row r and column c for flat index idx */
static inline void row_col(uint64_t idx, uint64_t *r, uint64_t *c) {
    /* binary search for r: largest r with r*(r+1)/2 <= idx */
    uint64_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= idx) {
        high <<= 1;            /* double until too large */
    }
    while (high - low > 1) {
        uint64_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= idx)
            low = mid;
        else
            high = mid;
    }
    *r = low;
    *c = idx - (uint64_t)(((unsigned __int128)low * (low + 1)) / 2);
}

seq_t a(idx_t n) {
    uint64_t r, c;
    row_col((uint64_t)n, &r, &c);

    uint64_t exp;
    if (c == 0) {                                   /* m = 0 case */
        exp = 2 * r - popcount_u64(r);
    } else {                                         /* m > 0 case */
        uint64_t k = r - c;                           /* k = n − m */
        uint64_t pop_k   = popcount_u64(k);
        uint64_t pop_rm1 = popcount_u64(r + c);        /* 2n − k = r + c */
        uint64_t pop_2n  = popcount_u64(r << 1);       /* 2n = 2*r */
        uint64_t v2 = pop_k + pop_rm1 - pop_2n;
        exp = (2 * r - 1) - v2;
    }

    return pow2(exp);                                 /* 2^exp */
}
