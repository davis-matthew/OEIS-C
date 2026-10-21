/*
 * A270027  a(n) is the smallest b >= 3 for which the base-b representation of
 *          n contains at least one 0 (or 0 if no such base exists).
 * https://oeis.org/A270027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 618, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A270027:
   a(n) = the smallest base b >= 3 such that the base‑b representation of n
   contains at least one digit 0.
   For n < 3 the function returns 0.
   The algorithm simply tests each base b in [3, n] in increasing order
   and checks whether any digit of n in that base is zero. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 3)          /* no base >= 3 can represent n with a zero digit */
        return 0;

    for (idx_t b = 3; b <= n; ++b) {
        idx_t temp = n;
        while (temp) {                 /* examine the digits of n in base b */
            if ((temp % b) == 0)       /* a zero digit found */
                return (seq_t)b;
            temp /= b;
        }
    }
    /* In theory this point is never reached because base n always works */
    return 0;
}
