/*
 * A393159  Square array A(n, k), n, k >= 0, read by antidiagonals: A(n, k) is
 *          the largest possible integer Euclidean distance between two
 *          lattice points with X-coordinates in the interval 0..n and
 *          Y-coordinates in the interval 0..k.
 * https://oeis.org/A393159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3788, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 107 terms through n = 9822 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A393159 – largest integer Euclidean distance between two lattice points
 *  with X‑coordinates 0..n and Y‑coordinates 0..k.
 *  The 1‑dimensional sequence a(n) is obtained by reading the square
 *  array A(i,j) along its antidiagonals:
 *   a(n) = A(i,j) where i+j = s,  i = n - s*(s+1)/2,  j = s - i.
 *  A is computed by dynamic programming using the recurrence
 *
 *   A(0,j) = j,          A(i,0) = i
 *   if i² + j² is a perfect square:  A(i,j) = √(i² + j²)
 *   otherwise:                 A(i,j) = max(A(i-1,j), A(i,j-1))
 *
 *  The algorithm runs in O(s²) with s≈√(2n) and meets the time limits.
 */
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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

#if defined(__cplusplus)
    #define ALLOC(N) new seq_t[(N)]
    #define FREE(p)  delete[] (p)
#else
    #define ALLOC(N) (seq_t*)malloc((size_t)(N)*sizeof(seq_t))
    #define FREE(p)  free(p)
#endif

seq_t a(idx_t n)
{
    /* find diagonal index s: s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = (idx_t)((sqrt((long double)(8 * n + 1)) - 1.0L) / 2.0L);
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;

    idx_t diag_start = s * (s + 1) / 2;
    idx_t offset     = n - diag_start;        /* offset within diagonal */
    idx_t i = offset;                          /* row index in array */
    idx_t j = s - offset;                      /* column index */

    /* handle trivial case s == 0 */
    if (s == 0) return 0;

    /* allocate two rows of size s+1 (0..s) */
    seq_t *prev = ALLOC(s + 1);
    seq_t *curr = ALLOC(s + 1);

    /* base row i = 0 : A(0,j) = j */
    for (idx_t jj = 0; jj <= s; ++jj) prev[jj] = (seq_t)jj;

    /* compute rows 1..i */
    for (idx_t ii = 1; ii <= i; ++ii) {
        curr[0] = (seq_t)ii;                   /* A(ii,0) = ii */
        for (idx_t jj = 1; jj <= s; ++jj) {
            seq_t sum = (seq_t)ii * ii + (seq_t)jj * jj;
            seq_t r   = floor_sqrt(sum);
            if (r * r == sum)
                curr[jj] = r;
            else
                curr[jj] = (prev[jj] > curr[jj-1]) ? prev[jj] : curr[jj-1];
        }
        /* swap rows for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[j];   /* A(i,j) with j = s - i */

    FREE(curr);
    FREE(prev);
    return result;
}
