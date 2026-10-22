/*
 * A334040  Number of odd numbers larger than n in the Collatz trajectory of
 *          n.
 * https://oeis.org/A334040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd numbers larger than n in the Collatz
 *            trajectory of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n < 1, return 0 (though the sequence is defined for n >= 1) */
    if (n < 1)
        return 0;

    seq_t count = 0;          /* Result to be returned */
    idx_t cur = n;            /* Current value in the Collatz sequence */

    /* Walk through the Collatz trajectory until reaching 1 */
    while (cur != 1) {
        if ((cur & 1) == 0) {            /* even */
            cur >>= 1;                  /* divide by 2 */
        } else {                         /* odd */
            cur = 3 * cur + 1;          /* 3*n + 1 */
        }
        /* Count odd terms that are strictly greater than the original n */
        if (cur > n && (cur & 1) == 1)
            ++count;
    }
    return count;
}
