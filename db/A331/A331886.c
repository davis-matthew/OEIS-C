/*
 * A331886  T(n, k) is the least positive m such that floor(n/m) = floor(k/m).
 *          Square array T(n, k) read by antidiagonals, n >= 0 and k >= 0.
 * https://oeis.org/A331886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) is the n-th term of the square array T(n,k) read by
 *            antidiagonals, where T(n,k) is the least positive integer m such
 *            that floor(n/m) = floor(k/m)
 * Range:     u8 n <= 9606, u16 n <= 9606, u32 n <= 9606, u64 n <= 9606,
 *            u128 n <= 9606, big any n
 * Helpers:   T_ij
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/T_ij.h"

seq_t a(idx_t n)
{
    /* Find the diagonal s such that
       s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while ((idx_t)((__int128)(s + 1) * (s + 2) / 2) <= n) ++s;

    /* Index within the diagonal: i + j = s. */
    idx_t before = (idx_t)((__int128)s * (s + 1) / 2);
    idx_t i = n - before;
    idx_t j = s - i;

    return T_ij(i, j);
}
