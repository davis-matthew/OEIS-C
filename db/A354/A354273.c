/*
 * A354273  Square array read by ascending antidiagonals: A(n,k) = k^Omega(n).
 * https://oeis.org/A354273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k^Ω(n) read by ascending antidiagonals with A(n,k) =
 *            k^Omega(n)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   bigomega, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Find diagonal length t (t = number of elements in the
       diagonal, also s-1 where s = i + k).  t is the smallest
       integer such that t*(t+1)/2 >= n. */
    idx_t t = 1;
    while (((unsigned __int128)t * (t + 1u)) / 2u < n)
        ++t;

    /* Number of terms before this diagonal */
    idx_t prev = ((unsigned __int128)(t - 1u) * t) / 2u;

    /* Zero‑based position within the diagonal */
    idx_t r = n - prev - 1u;

    /* Convert to array coordinates:
         i = t - r   (row index, where Omega is taken)
         k = r + 1   (column index). */
    idx_t i = t - r;
    idx_t k = r + 1u;

    /* Omega of i (count of prime factors, counting multiplicities) */
    seq_t omega_seq = bigomega(i);
    idx_t omega = (idx_t)omega_seq;   /* small enough for idx_t */

    /* Return k^Ω(i) */
    return pow_uint(k, omega);
}
