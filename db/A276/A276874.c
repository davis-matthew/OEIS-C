/*
 * A276874  Sums-complement of the Beatty sequence for sqrt(8).
 * https://oeis.org/A276874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer that is not equal to B[j] - B[i]
 *            for any 0 <= i < j, where B[k] = floor(k * sqrt(8))
 * Range:     u8 none, u16 none, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine uses the helper floor_sqrt to compute floor(k*sqrt(8))
   as floor_sqrt(8*k*k).  It then lists all differences up to a
   suitably large bound and returns the nth missing integer.  Complexity
   is O(K^2) with K ≈ 0.5*max_diff, which is well under the limits for
   n <= 10000. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* offset 1; never requested but safe */

    /* Upper bound M for the difference values we consider.  A generous
       constant (10*n+100) guarantees that the nth missing value lies
       below M for all n up to 10 000. */
    seq_t M = 10 * n + 100;
    size_t Msize = (size_t)(M + 1);

    /* Number of Beatty terms to generate.  Since the smallest
       difference is 2, any difference <= M can be obtained by a pair
       whose indices differ by at most M/2.  Therefore generating
       about M/2 + a small constant terms suffices. */
    idx_t K = (idx_t)(M / 2) + 5;     /* +5 for safety */

    /* Allocate the Beatty sequence array B[0 … K] */
    seq_t *B = mem_new(seq_t, (size_t)(K + 1));
    if (!B)
        return 0;                     /* memory allocation failed */

    B[0] = 0;
    for (idx_t i = 1; i <= K; ++i) {
        seq_t v = (seq_t)8 * (seq_t)i * (seq_t)i;      /* 8*i*i */
        B[i] = floor_sqrt(v);
    }

    /* Boolean array indicating whether a difference has been seen. */
    unsigned char *rep = (unsigned char *)calloc(Msize, sizeof(unsigned char));
    if (!rep) {
        mem_free(B);
        return 0;                     /* memory allocation failed */
    }

    /* Mark all differences B[j] - B[i] that are <= M. */
    for (idx_t i = 0; i < K; ++i) {
        for (idx_t j = i + 1; j <= K; ++j) {
            seq_t diff = B[j] - B[i];
            if (diff > M)
                break;                      /* further j will only increase diff */
            rep[(size_t)diff] = 1;
        }
    }

    /* Scan for missing numbers. */
    idx_t count = 0;
    seq_t result = 0;
    for (seq_t v = 1; v <= M; ++v) {
        if (!rep[(size_t)v]) {
            ++count;
            if (count == n) {
                result = v;
                break;
            }
        }
    }

    mem_free(B);
    free(rep);
    return result;
}
