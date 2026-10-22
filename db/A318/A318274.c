/*
 * A318274  Triangle read by rows: T(n,k) = n for 0 < k < n and T(n,0) =
 *          T(n,n) = 1.
 * https://oeis.org/A318274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the term obtained by reading the triangle T(n,k)
 *            (T(n,0)=T(n,n)=1, T(n,k)=n for 0<k<n) by rows. For an index n≥0,
 *            let r be the row containing the element: r = min{m ≥ 0 :
 *            (m+1)(m+2)/2 > n } . Let c = n – r(r+1)/2 (column index inside
 *            the row). Then a(n) = { 1, if c==0 or c==r r, otherwise...
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   prev_rowsum, t_rowsum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/prev_rowsum.h"
#include "../utils/t_rowsum.h"

static idx_t
find_row(idx_t n) /* minimal r with (r+1)(r+2)/2 > n */
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sum_mid = t_rowsum(mid);
        if (sum_mid > n)
            high = mid;
        else
            low = mid + 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* locate row r and column c ---------------------------------------- */
    idx_t r = find_row(n);
    idx_t prev = prev_rowsum(r);
    idx_t c = n - prev;

    /* evaluate the triangle entry --------------------------------------- */
    if (c == 0 || c == r)
        return (seq_t)1;
    else
        return (seq_t)r;
}
