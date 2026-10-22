/*
 * A330892  Square array of polygonal numbers read by descending antidiagonals
 *          (the transpose of A317302).
 * https://oeis.org/A330892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = P(r,c) where P(r,c) = (r-2) * (c*(c-1)/2) + c For each n,
 *            let d be the smallest non‑negative integer such that
 *            (d+1)(d+2)/2 ≥ n. Then r = n - d(d+1)/2 - 1 and c = d - r. a(n)
 *            is the value at position (r,c) in the array
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 by OEIS offset; values for n==0 are unspecified */
    if (n == 0) return 0;

    /* binary search for d such that (d+1)(d+2)/2 ≥ n */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;

    /* elements before diagonal d */
    unsigned __int128 before = (unsigned __int128)d * (d + 1) / 2;
    idx_t k = (idx_t)(n - before - 1);  /* 0‑based index inside the diagonal */

    idx_t r = k;
    idx_t c = d - k;

    seq_t r_s = (seq_t)r;
    seq_t c_s = (seq_t)c;

    seq_t cC1 = c_s * (c_s - 1) / 2;          /* c(c-1)/2 */
    seq_t result = (r_s - 2) * cC1 + c_s;     /* (r-2)*cC1 + c */

    return result;
}
