/*
 * A262581  Concatenation of the numbers from 1 to n but omitting 11.
 * https://oeis.org/A262581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of integers 1..m, where m = n + (n >= 11 ?
 *            1 : 0), omitting the integer 11
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digits.h"

seq_t a(idx_t n) {
    idx_t m = n;
    if (n >= 11) m = n + 1;
    seq_t result = 0;
    for (idx_t i = 1; i <= m; ++i) {
        if (i == 11) continue;
        result = result * pow10(digits(i)) + (seq_t)i;
    }
    return result;
}
