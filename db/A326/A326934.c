/*
 * A326934  Table of A(n,k) read by antidiagonals, where A(n,k) = (n*k) mod
 *          (n+1).
 * https://oeis.org/A326934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (i * j) mod (i + 1) where (i, j) are the coordinates of
 *            the n-th entry read by antidiagonals in the infinite matrix
 *            A(n,k) = (n*k) mod (n+1)
 * Range:     u8 n <= 1275, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   _diag_for_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_diag_for_idx.h"

seq_t a(idx_t n) {
    /* Determine diagonal and position within it */
    idx_t d   = _diag_for_idx(n);                 /* diagonal number */
    idx_t prev = (d - 1) * d / 2;                 /* entries before this diagonal */
    idx_t f    = n - prev;                        /* 1‑based position in diagonal */
    idx_t i    = f;                               /* row index (n in A(n,k))   */
    idx_t j    = d - f + 1;                       /* column index (k in A(n,k)) */

    /* Compute (i * j) mod (i + 1) safely using 128‑bit arithmetic */
    idx_t mod = i + 1;
    unsigned __int128 prod = (unsigned __int128)i * (unsigned __int128)(j % mod);
    seq_t result = (seq_t)(prod % mod);
    return result;
}
