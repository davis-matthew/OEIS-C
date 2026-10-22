/*
 * A349841  Triangle T(n,k) built by placing all ones on the left edge,
 *          [1,0,0,0,0] repeated on the right edge, and filling the body using
 *          the Pascal recurrence T(n,k) = T(n-1,k) + T(n-1,k-1).
 * https://oeis.org/A349841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = nth element of the triangle T(n,k) built by Pascal’s
 *            recurrence with T(n,0)=1, T(n,n)=1 when n mod 5 = 0 else 0
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function finds the row and column of the nth entry and builds
   the triangle up to that row. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

#ifdef __cplusplus
/* In C++ use new/delete for proper construction/destruction */

#else
/* In C use malloc/free */

#endif

seq_t a(idx_t n) {
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2   */
    uint64_t n64 = (uint64_t)n;
    uint64_t r;
    if (n64 == 0) {
        r = 0;
    } else {
        long double sq = sqrt((long double)(1 + 8 * (long double)n64));
        r = (uint64_t)((sq - 1.0L) / 2.0L);

        /* Correct possible rounding error */
        while (((__int128)(r + 1) * (r + 2) / 2) <= n64) ++r;
        while (((__int128)r * (r + 1) / 2) > n64) --r;
    }

    uint64_t k = n64 - (uint64_t)((__int128)r * (r + 1) / 2);

    seq_t *prev = NULL;
    seq_t *curr = NULL;

    for (uint64_t i = 0; i <= r; ++i) {
        curr = alloc_seq(i + 1);
        curr[0] = (seq_t)1;                     /* left edge */

        for (uint64_t j = 1; j < i; ++j)
            curr[j] = prev[j] + prev[j - 1];     /* Pascal recurrence */

        if (i > 0)
            curr[i] = ((i % 5ULL) == 0ULL) ? (seq_t)1 : (seq_t)0;   /* right edge */

        if (prev) free_seq(prev);
        prev = curr;
    }

    seq_t result = prev[k];
    free_seq(prev);
    return result;
}
