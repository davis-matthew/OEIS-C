/*
 * A296352  List of numbers k such that the determinant of the Unordered
 *          Lights Out matrix UBL_k is nonzero.
 * https://oeis.org/A296352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m ≥ 3 such that m % 5 != 0 && m % 6 != 0
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t k = 3;
    idx_t count = 0;
    for (;;) {
        if ((k % 5) != 0 && (k % 6) != 0) {
            ++count;
            if (count == n)
                return (seq_t)k;
        }
        ++k;
    }
}
