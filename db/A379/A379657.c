/*
 * A379657  The nonadjacent form of a(n) is obtained by inserting a digit 0
 *          before each nonzero digit in the balanced ternary expansion of n.
 * https://oeis.org/A379657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 7654, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) is defined by: the nonadjacent form (NAF) of a(n) is obtained
   by inserting a digit 0 before each nonzero digit in the balanced ternary
   expansion of n.  The function below implements this definition directly. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the special case n == 0 */
    if (n == 0) return 0;

    /* balanced ternary digits (least‑significant first) */
    int digits[128];
    size_t len = 0;
    idx_t temp = n;

    while (temp != 0) {
        int r = temp % 3;
        if (r == 0) {
            digits[len++] = 0;
            temp /= 3;
        } else if (r == 1) {
            digits[len++] = 1;
            temp = (temp - 1) / 3;
        } else { /* r == 2 */
            digits[len++] = -1;
            temp = (temp + 1) / 3;
        }
    }

    /* evaluate the NAF built from the digits: for each original digit
       (processed from most significant to least) we add a leading zero
       if the digit is non‑zero, then we append the digit itself.  */
    seq_t val = 0;
    for (size_t i = len; i > 0; --i) {
        int d = digits[i - 1];
        if (d == 0) {
            /* digit is zero: just shift left by 1 */
            val *= 2;
        } else {
            /* digit is ±1: shift left by 2 (insert zero) and add the digit */
            val *= 4;
            if (d == 1)
                val += 1;
            else                  /* d == -1 */
                val -= 1;
        }
    }

    return val;
}
