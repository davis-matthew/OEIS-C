/*
 * A376996  Number of odd terms in the Collatz trajectory of n which are > n
 *          and are a new record high among its odd terms.
 * https://oeis.org/A376996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the number of odd terms in the Collatz trajectory of n
   that are > n and are a new record high among such odd terms. */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t current = (seq_t)n;       /* current Collatz value */
    seq_t maxSeen = 0;              /* largest odd value > n seen so far */
    seq_t count   = 0;              /* number of new record odds */

    /* Iterate until we reach 1 */
    while (current != 1) {
        /* next Collatz step */
        if ((current & 1) == 0) {               /* even */
            current = current >> 1;             /* divide by 2 */
        } else {                                /* odd */
            current = current * 3 + 1;
        }

        /* Check if this odd term > n and is a new record */
        if ((current & 1) && current > n && current > maxSeen) {
            maxSeen = current;
            ++count;
        }
    }
    return count;
}
