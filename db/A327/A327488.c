/*
 * A327488  T(n, k) = 1 + NAND(k - 1, n - k), where NAND is the Sheffer stroke
 *          operating bitwise on the inputs, triangle read by rows, T(n, k)
 *          for n >= 1, 1 <= k <= n.
 * https://oeis.org/A327488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = element of the triangle defined by T(n,k)=1 +
 *            NAND(k-1,n-k)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   bitlen_seq_t_local, nand_bitwise
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   The sequence is obtained by reading the triangle row by row. */
#include <stdint.h>
#include "../utils/bitlen_seq_t_local.h"
#include "../utils/nand_bitwise.h"

/* public sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n < 1 */

    /* find row r such that r*(r+1)/2 >= n  (binary search) */
    idx_t low = 1, high = n;
    while (low < high)
    {
        idx_t mid = (low + high) / 2;
        idx_t cum = mid * (mid + 1) / 2;
        if (cum < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;

    idx_t cum_before = (r - 1) * r / 2;
    idx_t k = n - cum_before;                    /* column index, 1‑based */

    seq_t x = (seq_t)(k - 1);                    /* k-1   */
    seq_t y = (seq_t)(r - k);                    /* n-k   */
    seq_t val = nand_bitwise(x, y) + (seq_t)1;   /* 1 + NAND */

    return val;
}
