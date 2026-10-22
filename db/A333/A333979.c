/*
 * A333979  Array read by antidiagonals, n >= 0, k >= 2: T(n,k) is the
 *          "digital derivative" of n in base k; if the base k representation
 *          of n is Sum_{j>=0} d_j*k^j, then T(n,k) = Sum_{j>=1}
 *          d_j*j*k^(j-1).
 * https://oeis.org/A333979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) is the antidiagonal read of the table T(i,k) for i ≥ 0, k ≥
 *            2, where T(i,k) = floor(i/k) + k·T(floor(i/k),k)
 * Range:     u8 n <= 4751, u16 n <= 5049, u32 n <= 5049, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   digital_derivative
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by enumerating all pairs (i,k) in order of
   increasing i+k (the antidiagonals of the infinite table). */

#include <stddef.h>  /* for size_t if needed */
#include "../utils/digital_derivative.h"

/* Main entry point: returns a(n). */
seq_t a(idx_t idx)
{
    /* Find the diagonal s = i + k where idx lies. */
    idx_t s   = 2;      /* current diagonal sum (i + k), k ≥ 2 */
    idx_t cum = 0;      /* number of terms before this diagonal */

    while (1) {
        idx_t entries = s - 1;       /* count on diagonal s */
        if (cum + entries > idx)
            break;                   /* idx is on diagonal s */
        cum += entries;
        ++s;
    }

    idx_t offset = idx - cum;     /* offset within diagonal s */
    idx_t n_val  = offset;        /* row index i   */
    idx_t k_val  = s - offset;    /* column index k ≥ 2 */

    return digital_derivative(n_val, k_val);
}
