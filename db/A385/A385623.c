/*
 * A385623  Array read by ascending antidiagonals: A(n,k) is the number
 *          obtained by concatenation of n with k in that order, with k >= 0.
 * https://oeis.org/A385623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look, tabl
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 8673, u32 n <= 8673, u64 n <= 8673,
 *            u128 n <= 8673, big any n
 * Helpers:   digits_of, pow10
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
#include "../utils/pow10.h"
#include "../utils/digits_of.h"

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
    seq_t pow  = pow10(digits_of(col));
    seq_t val  = base * pow + (seq_t)col;
    return val;
}
