/*
 * A380114  Triangle read by rows: The convolution triangle of 2^n, where the
 *          convolution triangle of a sequence is defined in A357368.
 * https://oeis.org/A380114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the flattened convolution triangle of 2^m. For row r>0 :
 *            T(r,0)=0 For r=0 : T(0,0)=1 For 1<=k<=r : T(r,k)=2^r *
 *            binom(r-1, k-1)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 178, u64 n <= 611,
 *            u128 n <= 2305, big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* external types are defined by the build */
#include <stdint.h>
#include "../utils/pow2.h"   /* seq_t pow2(idx_t) */
#include "../utils/binom.h"  /* seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Special case: the very first term (row 0, column 0) */
    if (n == 0)
        return 1;

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2
       Using a binary search with 128‑bit arithmetic to avoid overflow. */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* Index of the first element of row r */
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;   /* column within the row */

    /* Row r>0, column 0 yields zero */
    if (k == 0)
        return 0;

    /* Compute T(r,k) = 2^r * binom(r-1, k-1) */
    seq_t bin = binom(r - 1, k - 1);
    seq_t val = bin * pow2(r);
    return val;
}
