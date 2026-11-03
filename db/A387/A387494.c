/*
 * A387494  Array read by antidiagonals: T(n,k) is the maximum number of steps
 *          in a n^k cuboid with a rule that allows moving to neighboring
 *          visited cuboids if the next step is onto an unvisited cuboid when
 *          starting in a corner.
 * https://oeis.org/A387494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, more
 * Formula:   a(n) = T(i,j) read along antidiagonals. * For the index n
 *            (starting at 1), find d = smallest integer with * d*(d-1)/2 >=
 *            n. * Let pos = n - (d-2)*(d-1)/2 (1‑based position on the
 *            diagonal). * Then i = pos, j = d - pos. * T(n,k) for n≥1, k≥1 is
 *            given (conjecturally exact) by: * T(n,k) = 2*n^k - k*...
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 15 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute T(n,k) for n≥1, k≥1. */
static inline seq_t T_func(idx_t n, idx_t k)
{
    if (n <= 1)          /* T(1,k) = 0 */
        return 0;

    seq_t nseq = (seq_t)n;
    seq_t kseq = (seq_t)k;

    /* Compute n^k. */
    seq_t npow = 1;
    for (idx_t t = 0; t < k; ++t)
        npow *= nseq;

    seq_t result = npow * (seq_t)2;          /* 2*n^k */
    result -= kseq * nseq;                   /* -k*n  */
    result += (seq_t)(k / n);                /* +floor(k/n) */

    if (k == 1)
        result -= 1;                         /* -δ(k==1) */
    if (n == 2 && k == 2)
        result -= 1;                         /* -δ(n==2)δ(k==2) */

    return result;
}

/* Find diagonal d for index n: smallest d with d*(d-1)/2 >= n. */
static inline idx_t diag_d(idx_t n)
{
    idx_t low = 2, high = 2;
    while ((unsigned __int128)high * (high - 1) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Integer square root for unsigned __int128 (floor). */
static inline unsigned __int128 floor_sqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = (((unsigned __int128)1) << 64);
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function.  Decode (i,j) from the antidiagonal index n
 * and return T(i,j). */
seq_t a(idx_t n)
{
    /* Locate the diagonal d. */
    idx_t d = diag_d(n);

    /* Previous cumulative count before this diagonal: (d-2)*(d-1)/2. */
    unsigned __int128 prev =
        ((unsigned __int128)(d - 2) * (d - 1)) / 2;

    /* Position within the diagonal (1‑based). */
    idx_t pos = n - (idx_t)prev;

    /* Coordinates on the matrix. */
    idx_t i = pos;        /* row index */
    idx_t j = d - pos;    /* column index, decreases as i increases */

    return T_func(i, j);
}
