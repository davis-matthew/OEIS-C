/*
 * A257570  Rectangular array, read by antidiagonals: d(h,k) = distance
 *          between h and k in the tree at A232558, for h >=0, k >= 0.
 * https://oeis.org/A257570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the length of the unique path between the vertices h and
 *            k in the infinite tree T whose vertices are the non‑negative
 *            integers. In T every vertex x>0 has a single neighbour toward
 *            the root 0 given by parent(x) = x-1 if x is odd x/2 if x is even
 *            The pair (h,k) corresponding to the index n...
 * Range:     u8 n <= 1830, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   depth, parent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/depth.h"
#include "../utils/parent.h"

/* 128‑bit integer square root (floor)  */
static inline idx_t isqrt_u128(unsigned __int128 val) {
    idx_t lo = 0;
    idx_t hi = (idx_t)(val >> 1) + 1;          /* hi is a safe upper bound */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= val) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n) {
    if (n == 0)          /* indices < 1 are out of the offset */
        return (seq_t)0;

    /* 1. map the index to the pair (h,k) by antidiagonal enumeration */
    idx_t t = n - 1;                           /* zero‑based index */
    unsigned __int128 val = (unsigned __int128)8 * t + 1;
    idx_t sqrtD = isqrt_u128(val);
    idx_t s     = (sqrtD - 1) / 2;             /* h + k */
    idx_t start = (s * (s + 1)) / 2;           /* number of pairs before this diagonal */
    idx_t r     = t - start;                   /* offset inside the diagonal */
    idx_t h = r;
    idx_t k = s - r;

    /* 2. compute the distance in the tree */
    if (h == k)
        return (seq_t)0;

    idx_t dh = depth(h);
    idx_t dk = depth(k);
    idx_t a1 = h, a2 = k;
    seq_t dist = 0;

    /* lift the deeper node until both depths are equal */
    while (dh > dk) {
        a1 = parent(a1);
        dh--;
        dist++;
    }
    while (dk > dh) {
        a2 = parent(a2);
        dk--;
        dist++;
    }

    /* lift both nodes together until they meet at the LCA */
    while (a1 != a2) {
        a1 = parent(a1);
        a2 = parent(a2);
        dist += 2;
    }

    return dist;
}
