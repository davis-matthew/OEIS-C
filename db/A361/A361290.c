/*
 * A361290  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=0..floor((n-1)/2)} k^(n-1-j) *
 *          binomial(n,2*j+1).
 * https://oeis.org/A361290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   di_from_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A361290
 *
 *  Square array T(n,k) is read by antidiagonals downwards.
 *
 *  T(0,k) = 0                       (k ≥ 0)
 *  T(1,k) = 1
 *  For n ≥ 2:  T(n,k) = 2*k*T(n-1,k) - (k-1)*k*T(n-2,k)
 *
 *  a(n) is the element of that array whose diagonal index i+j
 *  satisfies:
 *      d = max{ m | m*(m+1)/2 ≤ n }      (d ≥ 0)
 *      i = n - d*(d+1)/2                (row index)
 *      j = d - i                        (column index)
 *  Then a(n) = T(i,j).
 *
 *  The implementation uses the recurrence directly, avoiding
 *  any table or floating‑point arithmetic.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/di_from_n.h"

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Determine (i,j) corresponding to n */
    idx_t d   = di_from_n(n);
    idx_t before = d * (d + 1) / 2;
    idx_t i = n - before;
    idx_t j = d - i;

    /* Special handling for column 0 to avoid unsigned underflow */
    if (j == 0) {
        return (i == 0) ? (seq_t)0
             : (i == 1) ? (seq_t)1
                        : (seq_t)0;
    }

    /* Base values for the recurrence */
    seq_t prev = (seq_t)0;   /* T(0,j) */
    seq_t curr = (seq_t)1;   /* T(1,j) */

    if (i == 0) return prev;
    if (i == 1) return curr;

    /* Iterate the recurrence up to i */
    for (idx_t t = 2; t <= i; ++t) {
        seq_t term1 = (seq_t)2 * (seq_t)j * curr;
        seq_t term2 = (seq_t)((j - 1) * j) * prev;
        seq_t next  = term1 - term2;
        prev = curr;
        curr = next;
    }
    return curr;
}
