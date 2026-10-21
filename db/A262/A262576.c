/*
 * A262576  Concatenation of the numbers from 1 to n but omitting 6.
 * https://oeis.org/A262576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the integers from 1 to M(n), where M(n)
 *            = n if n < 6 else n + 1, omitting the number 6 entirely
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digits_of.h"
#include "../utils/pow10.h"


seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t M = (n < 6) ? n : n + 1;
    for (idx_t k = 1; k <= M; ++k) {
        if (k == 6)
            continue;
        int d = digits_of(k);          /* number of decimal digits of k */
        seq_t pow_val = pow10(d);      /* 10^d as seq_t */
        result = result * pow_val + (seq_t)k;
    }
    return result;
}
