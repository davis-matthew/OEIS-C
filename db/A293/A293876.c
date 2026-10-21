/*
 * A293876  Numbers having '16' as substring of their digits / decimal
 *          expansion.
 * https://oeis.org/A293876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2989, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has16
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes the sequence of positive integers whose decimal representation
 * contains the substring "16".  a(n) is the n‑th such integer with a(1)=16.
 *
 * The implementation performs a simple scan of consecutive integers,
 * checking each for the presence of the pattern using only integer
 * operations (no string conversion, no floating‑point).  This is
 * sufficient for n up to at least 10 000 within the required time.
 */

#include <stdbool.h>
#include "../utils/has16.h"

seq_t a(idx_t n)
{
    /* a(1) = 16 */
    seq_t current = (seq_t)16;
    if (n == 1)
        return current;

    seq_t count = 1;   /* number of found terms so far */

    for (;;) {
        ++current;                       /* look at the next integer */
        if (has16(current)) {
            ++count;
            if (count == n)
                return current;          /* found the n‑th term */
        }
    }
}
