/*
 * A281494  Number of ordered ways to write n as x^2 + y^2 + z^2 + w^2 with x
 *          + y + z + w = 2^(floor((ord_2(n)+1)/2)), where ord_2(n) is the
 *          2-adic order of n, and x,y,z,w are integers with |x| <= |y| <= |z|
 *          <= |w|.
 * https://oeis.org/A281494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration method for OEIS A281494:
   Count ordered quadruples (x,y,z,w) of integers with |x|≤|y|≤|z|≤|w|,
   x²+y²+z²+w² = n and x+y+z+w = 2^floor((ord₂(n)+1)/2). */
#include <stdint.h>
#include <stdbool.h>

/* Integer floor square root for 64‑bit unsigned numbers */
static inline idx_t floor_isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            res = 0;
            lo = 1;
            continue;
        }
        if (mid <= n / mid) {
            res = mid;
            lo = mid + 1;
        } else
            hi = mid - 1;
    }
    return res;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* compute value of S = 2^floor((ord2+1)/2) */
    idx_t tmp = n;
    int ord2 = 0;
    while ((tmp & 1ULL) == 0) {
        ord2++;
        tmp >>= 1;
    }
    int t = (ord2 + 1) >> 1;
    idx_t S = 1ULL << t;

    seq_t total = 0;

    for (idx_t a = 0; a * a <= n; a++) {
        idx_t rem1 = n - a * a;
        for (idx_t b = a; b * b <= rem1; b++) {
            idx_t rem2 = rem1 - b * b;
            for (idx_t c = b; c * c <= rem2; c++) {
                idx_t rem3 = rem2 - c * c;
                idx_t d = floor_isqrt_u64(rem3);
                if (d < c) continue;          /* maintain ordering */
                if (d * d != rem3) continue;  /* not a perfect square */

                /* abs values are sorted: a <= b <= c <= d */
                idx_t absVals[4] = {a, b, c, d};
                idx_t T = a + b + c + d;

                /* parity check for possible sign assignment */
                if (((T + S) & 1ULL) != 0) continue;

                idx_t target = (T + S) >> 1;

                /* collect indices of non‑zero values */
                idx_t nzIdx[4];
                int nzCnt = 0;
                for (int i = 0; i < 4; i++)
                    if (absVals[i] != 0) nzIdx[nzCnt++] = i;

                seq_t ways = 0;
                /* iterate over all sign choices for non‑zero entries */
                for (idx_t mask = 0; mask < ((idx_t)1 << nzCnt); mask++) {
                    idx_t sumPos = 0;
                    for (int j = 0; j < nzCnt; j++) {
                        if (mask & ((idx_t)1 << j))
                            sumPos += absVals[nzIdx[j]];
                    }
                    if (sumPos == target) ways++;
                }
                total += ways;
            }
        }
    }
    return total;
}
