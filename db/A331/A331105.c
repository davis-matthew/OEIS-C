/*
 * A331105  T(n,k) = -k*(k+1)/2 mod 2^n; triangle T(n,k), n>=0, 0<=k<=2^n-1,
 *          read by rows.
 * https://oeis.org/A331105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, tabf
 * Formula:   a(n) = T(r,k) where * r = ⌊log₂(n+1)⌋, k = n – (2^r – 1) *
 *            T(r,k) = –k·(k+1)/2 (mod 2^r)
 * Range:     u8 n <= 511, u16 n <= 9675, u32 n <= 9675, u64 n <= 9675,
 *            u128 n <= 9675, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Determine the row r and offset k inside that row.  */
    idx_t r = floor_log2_uint64(n + 1);            /* r = floor(log2(n+1)) */
    idx_t prev_len = ((idx_t)1 << r) - 1;          /* total entries before row r */
    idx_t k = n - prev_len;                       /* column index within row r */

    /* Compute k*(k+1)/2  (unrestricted, but values are small). */
    seq_t val = (seq_t)k * ((seq_t)k + 1) / 2;

    /* Mask for r bits: 2^r – 1.  For r==0 the mask is 0. */
    seq_t mask = (r == 0) ? 0 : (((seq_t)1 << r) - 1);

    /* Take only the lowest r bits of the triangular term. */
    seq_t t = val & mask;

    /* The required value is –t (mod 2^r), i.e., 2^r – t, reduced to r bits. */
    seq_t result = (((seq_t)1 << r) - t) & mask;

    return result;
}
