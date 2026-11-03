/*
 * A385623  Array read by ascending antidiagonals: A(n,k) is the number
 *          obtained by concatenation of n with k in that order, with k >= 0.
 * https://oeis.org/A385623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look, tabl
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 11475, u32 n <= 11475, u64 n <= 11475,
 *            u128 n <= 11475, big any n
 * Validated: standard, pass, 94 terms through n = 8673 (2026-11-01)
 * Helpers:   num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A385623: array read by ascending antidiagonals
   For n >= 0, a(n) is obtained by enumerating the grid
   A(i,j) = concatenate(i,j) for i>=0, j>=0 along
   diagonals of constant i+j, starting at (0,0),
   and reading each diagonal from i = d down to 0. */

/* For the construction we need to find the diagonal d
   such that d(d+1)/2 <= n < (d+1)(d+2)/2.  The offset
   within the diagonal is n - d(d+1)/2, giving
   row = d - offset, col = offset.  The value is
   row * 10^{digits(col)} + col, where digits(0)=1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/num_digits_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Find the diagonal index d for a given n */
static inline idx_t diagonal_of(idx_t n)
{
    idx_t lo = 0, hi = 1;
    /* Expand hi until it's too large */
    while (( (__int128)hi*(hi+1)/2 ) <= n) {
        /* Prevent infinite loop if n is near ULLONG_MAX */
        if (hi > (UINT64_MAX >> 1)) break;
        hi <<= 1;
    }
    /* Binary search for largest d with d(d+1)/2 <= n */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1)/2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    idx_t d = diagonal_of(n);
    __int128 diag_start = (__int128)d * (d + 1) / 2;
    idx_t offset = n - (idx_t)diag_start;  /* offset within diagonal */
    idx_t row  = d - offset;               /* i */
    idx_t col  = offset;                   /* j */

    seq_t base = (seq_t)row;
    seq_t pow  = pow10(num_digits_idx(col));
    seq_t val  = base * pow + (seq_t)col;
    return val;
}
