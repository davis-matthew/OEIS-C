/*
 * A285898  Triangle read by row: T(n,k) = number of partitions of n into
 *          exactly k consecutive parts (1 <= k <= n).
 * https://oeis.org/A285898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 136, u16 n <= 136, u32 n <= 136, u64 n <= 136,
 *            u128 n <= 136, big any n
 * Helpers:   ceil_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ceil_row.h"
/* Sequence A285898: triangle of partitions into k consecutive parts,
   read by row.  A(n) is 1 if the row/column indexed by the n‑th
   entry corresponds to a valid partition, otherwise 0.  The entry
   at position n belongs to row i and column j where
      i*(i-1)/2 < n ≤ i*(i+1)/2.
   For that row i (the sum) and column j (the number of parts k),
   there is a partition iff (i - k*(k-1)/2) > 0 and is divisible by k.
*/

seq_t a(idx_t n) {
    /* For safety handle n == 0 (should not occur, offset 1). */
    if (n == 0)
        return 0;

    idx_t i = ceil_row(n);  /* row index = sum */

    /* triangular number of previous rows: (i-1)*i/2 */
    unsigned __int128 prev128 = (unsigned __int128)(i - 1) * i / 2;
    idx_t prev = (idx_t)prev128;

    idx_t j = n - prev;   /* column index (1 <= j <= i) */
    idx_t k = j;          /* number of parts */

    /* Compute t = k*(k-1)/2 */
    signed __int128 t128 = (signed __int128)k * (k - 1) / 2;

    /* Compute m = i - t, must be > 0 and divisible by k */
    signed __int128 m128 = (signed __int128)i - t128;
    if (m128 <= 0)
        return 0;
    if ((m128 % (signed __int128)k) == 0)
        return (seq_t)1;
    return (seq_t)0;
}
