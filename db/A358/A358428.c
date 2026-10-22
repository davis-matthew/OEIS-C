/*
 * A358428  Numbers k such that k^2 + 1, k^2 + 2 and k^2 + 3 are all
 *          squarefree.
 * https://oeis.org/A358428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Function a(n) :  OEIS A358428
 *  The function returns the nth integer k (1‑based index) such that
 *  all of k^2+1, k^2+2, k^2+3 are squarefree.
 *  It does so by brute‑force scanning until the desired count is reached.
 *  The algorithm is O(k) where k ~ 3.18*n, easily fast for n up to 10⁴.
 */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    idx_t      count = 0;
    seq_t      k = 0;

    while (count < n) {
        ++k;                                 /* candidate k  */
        seq_t sq = k * k;                    /* k²          */
        if (is_squarefree(sq + 1) &&
            is_squarefree(sq + 2) &&
            is_squarefree(sq + 3)) {
            ++count;
        }
    }
    return k;
}
