/*
 * A322936  Triangular array in which the n-th row lists the numbers strongly
 *          prime to n (in ascending order). For the empty rows n = 2, 3, 4
 *          and 6 we set by convention 0.
 * https://oeis.org/A322936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the concatenation of the rows of *
 *            numbers ≤row that are coprime to the row index and do not divide
 *            * (row−1); rows with no such numbers contribute a single 0. Row
 *            1 * is defined to contain {1}
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   uint64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/uint64_gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, not used by OEIS offset 1 */

    idx_t count = 0;                /* number of terms output so far */

    for (idx_t row = 1; ; ++row) {
        if (row == 1) {             /* special case: row 1 = {1} */
            ++count;
            if (count == n) return (seq_t)1;
            continue;
        }

        bool has_value = false;

        for (idx_t a_val = 1; a_val <= row; ++a_val) {
            if (uint64_gcd(a_val, row) == 1 && ((row - 1) % a_val != 0)) {
                ++count;
                if (count == n) return (seq_t)a_val;
                has_value = true;
            }
        }

        if (!has_value) {           /* empty row → output 0 */
            ++count;
            if (count == n) return (seq_t)0;
        }
    }
}
