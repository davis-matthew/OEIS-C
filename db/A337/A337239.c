/*
 * A337239  Numbers having at least one 7 in their representation in base 8.
 * https://oeis.org/A337239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer whose base‑8 representation contains at
 *            least one digit 7. Offset 1 (a(1) = 7)
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9287 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if x has the digit 7 in base 8. */
static bool has7(seq_t x)
{
    while (x > (seq_t)0) {
        if ((x % (seq_t)8) == (seq_t)7)
            return true;
        x /= (seq_t)8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)        /* undefined for the offset 1 */
        return (seq_t)0;

    idx_t found = (idx_t)0;
    seq_t val  = (seq_t)1;

    while (found < n) {
        if (has7(val)) {
            ++found;
            if (found == n)
                return val;
        }
        ++val;
    }

    /* Should never be reached for a valid n ≥ 1.  Return 0 to silence
       compiler warnings. */
    return val;
}
