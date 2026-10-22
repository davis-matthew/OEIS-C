/*
 * A390723  Triangle read by rows: T(n, k) = (-1)^(n-k)*Stirling1(n,
 *          k)*CatalanNumber(k).
 * https://oeis.org/A390723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n)-th entry of the triangle T(r,k) = Stirling1(r,k)
 *            * Catalan(k) (unsigned Stirling numbers of the first kind) The
 *            triangle is read row by row starting with n = 0. r =
 *            floor((sqrt(8*n+1)-1)/2), k = n - r*(r+1)/2. For r=0,k=0:
 *            T(0,0)=1, otherwise T(r,k)=c(r,k)*Catalan(k)
 * Range:     u8 n <= 22, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   catalan_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/catalan_calc.h"

static seq_t *stirling = NULL;      /* triangular array of unsigned Stirling numbers of the first kind */
static idx_t  stirling_rows = 0;    /* number of rows currently stored (rows 0..stirling_rows-1) */

/* Compute floor(sqrt(n)) for 64‑bit unsigned integers using binary search.
   This works for arbitrary precision seq_t as we only use it for 64‑bit indices. */
static inline idx_t uint64_floor_sqrt(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Ensure that the array of Stirling numbers contains rows 0..n (inclusive).  */
static void ensure_stirling(idx_t n)
{
    if (stirling_rows > n) return;          /* already enough */

    idx_t old_rows = stirling_rows;
    idx_t new_rows = n + 1;                  /* rows needed */
    size_t new_cap = (size_t)new_rows * (new_rows + 1) / 2;  /* triangular number */

    seq_t *new_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (stirling_rows > 0) {
        size_t old_cap = (size_t)stirling_rows * (stirling_rows + 1) / 2;
        memcpy(new_arr, stirling, old_cap * sizeof(seq_t));
        free(stirling);
    }
    stirling = new_arr;

    /* Compute and append new rows */
    for (idx_t r = old_rows; r < new_rows; ++r) {
        size_t idx = (size_t)r * (r + 1) / 2;       /* start of row r */

        if (r == 0) {                              /* row 0 : only 1 */
            stirling[0] = 1;
            continue;
        }

        size_t prev_idx = (size_t)(r - 1) * r / 2;   /* start of previous row */

        stirling[idx] = 0;                          /* k = 0, c(r,0)=0 for r>0 */

        for (idx_t k = 1; k <= r; ++k) {
            seq_t term1 = stirling[prev_idx + (k - 1)];   /* c(r-1,k-1) */
            seq_t term2 = (seq_t)(r - 1) * stirling[prev_idx + k]; /* (r-1)*c(r-1,k) */
            stirling[idx + k] = term1 + term2;
        }
    }
    stirling_rows = new_rows;
}

/* Public function required by the test harness */
seq_t a(idx_t n)
{
    /* Determine the row r and column k of the n-th entry in the triangle. */
    idx_t t = (idx_t)8 * n + 1;
    idx_t s = uint64_floor_sqrt(t);
    idx_t r = (s - 1) / 2;
    size_t start = (size_t)r * (r + 1) / 2;
    idx_t k = n - (idx_t)start;

    ensure_stirling(r);
    seq_t cat = catalan_calc(k);

    if (r == 0 && k == 0)               /* special case: a(0)=1 */
        return cat;

    size_t idx = (size_t)r * (r + 1) / 2 + k;
    return stirling[idx] * cat;
}
