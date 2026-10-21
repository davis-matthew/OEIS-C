/*
 * A275984  Decimal expansion of 2^44497 - 1, the 27th Mersenne prime
 *          A000668(27).
 * https://oeis.org/A275984
 *
 * Offset:    13395    (a(13395) is the first term)
 * Keywords:  nonn, cons, easy, fini, full
 * Formula:   a(n) = the n-th decimal digit of 2^44497 – 1
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   pow2, seq_to_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/seq_to_str.h"

seq_t a(idx_t n)
{
    static bool init = false;
    static char digits[20050];      /* plenty of space for the 13382 digits */
    static size_t digits_len = 0;

    if (!init) {
        seq_t m = pow2(44497);      /* compute 2^44497 */
        m -= 1;                    /* subtract 1 to get the Mersenne prime */
        seq_to_str(m, digits, &digits_len);
        init = true;
    }

    /* The sequence is defined for n >= 13395; behavior below that is arbitrary. */
    if (n == 0 || n > digits_len) {
        return (seq_t)0;
    }
    return (seq_t)(digits[n - 1] - '0'); /* return the digit as an integer */
}
