/*
 * A321312  A(n,k) = n^^k is the k-th tetration of n; square array A(n,k),
 *          n>=0, k>=0, read by antidiagonals.
 * https://oeis.org/A321312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 22, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A321312:
   a(n) is the antidiagonal traversal of the square array
   A(n,k) = n^^k   (n tetrated to height k).  The tetration
   is defined recursively: n^^0 = 1, n^^(k+1) = n^(n^^k).  */
#include <stdint.h>
#include "../utils/big_pow.h"

/* Compute n^^k for integer n ≥ 0 and k ≥ 0.  */
static seq_t tetration(idx_t base, idx_t exp)
{
    if (exp == 0)
        return (seq_t)1;                     /* n^^0 = 1  */

    /* Special handling for base 0: 0^^1 = 0,
       0^^k = 1 if k is even, 0 if k is odd (k > 0). */
    if (base == 0) {
        if (exp == 1)
            return (seq_t)0;
        return ((exp & 1) == 0) ? (seq_t)1 : (seq_t)0;
    }

    /* For base 1, every tetration value is 1. */
    if (base == 1)
        return (seq_t)1;

    /* General case: n^^1 = n, and for k ≥ 2 we use
       the recurrence n^^k = n^(n^^(k-1)).  */
    seq_t result = (seq_t)base;                 /* n^^1 */
    for (idx_t k = 2; k <= exp; ++k) {
        result = big_pow((seq_t)base, result);  /* n^result */
    }
    return result;
}

/* Antidiagonal coordinates:
   for a given index n (starting from 0) find e such that
   e*(e+1)/2 ≤ n < (e+1)*(e+2)/2.
   The position inside diagonal e is then idx = n - e*(e+1)/2.
   This gives row = idx, column = e - idx. */
seq_t a(idx_t n)
{
    /* Find the largest e satisfying e*(e+1)/2 ≤ n. */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tmid = (unsigned __int128)mid * (mid + 1) / 2;
        if (tmid <= n) {
            low = mid + 1;
        } else {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    idx_t e = high;
    unsigned __int128 t_e = (unsigned __int128)e * (e + 1) / 2;
    idx_t idx = n - (idx_t)t_e;                 /* position inside diagonal e */

    idx_t row = idx;          /* i */
    idx_t col = e - idx;      /* j */

    return tetration(row, col);
}
