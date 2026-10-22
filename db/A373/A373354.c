/*
 * A373354  Triangle read by rows: T(n, k) = [n - k + 1 | k] where [n | k] is
 *          defined below.
 * https://oeis.org/A373354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   is_qr_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A373354:
   Triangle read by rows:  T(n,k) = [n‑k+1 | k]
   where [x|y] is defined by quadratic residues.
   a(n) is the n-th term of the triangle when read row‑wise (offset 1). */

#include <stdbool.h>
#include "../utils/is_qr_mod.h"

/* Return the row r such that r*(r+1)/2 >= n  (the smallest such r). */
static inline idx_t triangle_row(idx_t n)
{
    idx_t lo = 1, hi = 2;
    /* Find an upper bound */
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;
    /* Binary search for the minimal r */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Row and column within the triangle */
    idx_t r = triangle_row(n);                     /* row number, 1-based */
    idx_t prev = (r - 1) * r / 2;                  /* number of terms before row r */
    idx_t k = n - prev;                            /* column number in row r, 1‑based */
    idx_t m = r - k + 1;                           /* argument of [·|·] */

    bool mRk = is_qr_mod(m, k);                    /* m is residue mod k   */
    bool kRm = is_qr_mod(k, m);                    /* k is residue mod m   */

    if (!mRk && !kRm) return (seq_t)0;
    if (mRk && kRm)  return (seq_t)1;
    if (mRk && !kRm) return (seq_t)2;
    return (seq_t)3;  /* !mRk && kRm */
}
