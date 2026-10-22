/*
 * A357915  Concatenation of the decimal digits of {n, 1..n}.
 * https://oeis.org/A357915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(n, A007908(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   int_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/int_digits.h"

/* Concatenate the decimal digits of n followed by the concatenation of 1..n.
   Returns the result as a seq_t value. */
seq_t a(idx_t n)
{
    /* Pre‑compute powers of ten up to 10^24 once. */
    static seq_t pow10[25];
    static bool inited = false;
    if (!inited) {
        pow10[0] = (seq_t)1;
        for (int i = 1; i < 25; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;
        inited = true;
    }

    seq_t result = (seq_t)n;
    for (idx_t i = 1; i <= n; ++i) {
        int d = int_digits(i);      /* number of digits of i */
        result = result * pow10[d] + (seq_t)i;
    }
    return result;
}
