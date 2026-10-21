/*
 * A276349  Numbers consisting of a nonempty string of 1's followed by a
 *          nonempty string of 0's.
 * https://oeis.org/A276349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (10^i - 1)/9 * 10^j where i + j = d, d = smallest integer
 *            with d(d-1)/2 ≥ n, j = d - (n - (d-2)(d-1)/2) (j = 1 … d-1)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 45, u64 n <= 190, u128 n <= 741,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* Find the smallest k such that k(k+1)/2 ≥ n.  k is used as a
   helper to compute the digit‑length d = k+1. */
static inline idx_t triangular_index(idx_t n)
{
    idx_t k = 1;
    while ( ((unsigned __int128)k * (k + 1)) / 2 < n )
        ++k;
    return k;
}

seq_t a(idx_t n)
{
    /* 1. Determine the block (total digits d) containing n. */
    idx_t k = triangular_index(n);   /* k(k+1)/2 ≥ n  and  k-1 insufficient   */
    idx_t d = k + 1;                  /* i + j = d  (d ≥ 2) */

    /* 2. Count how many terms precede this block. */
    idx_t prev = ((d - 2) * (d - 1)) / 2;   /* terms with total digits < d */

    /* 3. Position inside the current block (1‑based). */
    idx_t pos = n - prev;   /* 1 .. d-1 */

    /* 4. Compute numbers of ones and zeros. */
    idx_t j = d - pos;           /* trailing zeros (j = d-1 … 1) */
    idx_t i = d - j;             /* leading ones              */

    /* 5. Build the value: (10^i - 1)/9  followed by j zeros. */
    seq_t ones  = pow10(i) - 1;   /* 10^i - 1  */
    ones /= 9;                   /* (10^i - 1)/9 => i ones */
    seq_t zeros = pow10(j);       /* 10^j      => j zeros */

    return ones * zeros;
}
