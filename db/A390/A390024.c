/*
 * A390024  Values of u in the quartets (4, u, v, w) of type 3; i.e., values
 *          of u for solutions to 4*(4 - u) = v*(v - w), in distinct positive
 *          integers, with v > 1, sorted by nondecreasing values of u; see
 *          Comments.
 * https://oeis.org/A390024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A390024.
   For each positive integer u ≠ 4, we test whether there exist
   distinct positive integers v>4 and w such that 4·(4−u)=v·(v−w).
   Each admissible pair (u,v) contributes one term u to the sequence.
   The sequence is produced in nondecreasing order of u, duplicating
   u for every admissible v.  The function returns the n-th term
   (1‑based index).  The implementation enumerates u incrementally
   and stops as soon as the required number of terms have been found. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Integer square root of a 64‑bit unsigned integer */
static inline idx_t isqrt_u64(uint64_t n) {
    idx_t low = 0, high = (idx_t)0xFFFFFFFFFFFFFFFFULL;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {
            if (n == 0) return 0;
            low = 1;  /* avoid division by zero in n/mid */
            continue;
        }
        if (mid > n / (uint64_t)mid) /* mid^2 > n without overflow */
            high = mid - 1;
        else {
            if ((uint64_t)mid * (uint64_t)mid == n) return mid;
            low = mid + 1;
        }
    }
    return low - 1;
}

/* Return the n-th term of A390024 (1‑based index) */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined, but safe */

    idx_t count = 0;

    for (idx_t u = 1; ; ++u) {
        if (u == 4) continue;                 /* u must be distinct from m=4 */

        long long L = 16 - 4LL * (long long)u;   /* 4·(4−u) */
        if (L == 0) continue;                   /* impossible, u=4 */

        uint64_t absL = (L >= 0) ? (uint64_t)L : (uint64_t)(-L);
        idx_t root = isqrt_u64(absL);

        for (idx_t d = 1; d <= root; ++d) {
            if (absL % d != 0) continue;

            /* first divisor */
            idx_t v = d;
            if (v > 4) {
                long long k = L / (long long)v;     /* L/v, exact division */
                long long w = (long long)v - k;
                if (w > 0 && w != 4LL && w != (long long)u && (long long)u != (long long)v) {
                    ++count;
                    if (count == n) return (seq_t)u;
                }
            }

            /* paired divisor */
            idx_t v2 = absL / d;
            if (v2 != d && v2 > 4) {
                long long k = L / (long long)v2;
                long long w = (long long)v2 - k;
                if (w > 0 && w != 4LL && w != (long long)u && (long long)u != (long long)v2) {
                    ++count;
                    if (count == n) return (seq_t)u;
                }
            }
        }
    }
}
