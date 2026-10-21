/*
 * A296612  Square array read by antidiagonals upwards: T(n,k) equals k times
 *          the number of compositions (ordered partitions) of n, with n >= 0
 *          and k >= 1.
 * https://oeis.org/A296612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sequence obtained by reading upward *
 *            anti‑diagonally the square array T(n,k) = k * A011782(n), *
 *            where A011782(0) = 1 and A011782(n) = 2^(n-1) for n > 0
 * Range:     u8 n <= 44, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

/* Return the n-th element (0‑based) of A296612. */
seq_t a(idx_t n)
{
    /* 1. Find the diagonal `s` such that
     *    sum_{t=1}^{s-1} t <= n < sum_{t=1}^s t.
     *    `cum` holds sum_{t=1}^s t after the loop. */
    idx_t s   = 1;   /* current diagonal (starts at 1) */
    idx_t cum = 1;   /* sum of first s diagonals */

    while (cum <= n) {
        ++s;
        cum += s;     /* add the next diagonal length */
    }

    /* 2. Compute offset within the diagonal. */
    idx_t offset     = n - (cum - s);           /* 0-based offset */
    idx_t n_coord    = (s - 1) - offset;        /* row index (n) */
    idx_t k_coord    = s - n_coord;             /* column index (k) */

    /* 3. Number of compositions of n_coord. */
    seq_t comps = (n_coord == 0) ? (seq_t)1
                                 : pow_uint(2, (idx_t)n_coord - 1);

    /* 4. Result is k_coord * comps. */
    return (seq_t)k_coord * comps;
}
