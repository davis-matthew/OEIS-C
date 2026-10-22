/*
 * A344208  Numbers k such that iterating x -> digsum(x)^2 + 1 from k one or
 *          more times results in a number < 10.
 * https://oeis.org/A344208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function returns the nth integer k (n ≥ 1) that eventually reaches
   a single‑digit number by iterating the map
          f(x) = digsum(x)^2 + 1
   (at least one iteration is required).  The sequence is the OEIS
   A344208.  If a number falls into the cycle 65 → 122 → 26 → 65 it
   does **not** belong to the sequence.  The implementation uses only
   integer arithmetic within the provided utilities.
*/
#include <stdbool.h>
#include "../utils/digit_sum.h"

static bool terminates(seq_t x)
/* Return true iff iterating f on x eventually produces a single‑digit
   number (hence reaches a value < 10).  A cycle involving 65, 122,
   26 is detected and considered non‑terminating. */
{
    seq_t y;
    /* Perform at least one iteration */
    do {
        seq_t s = digit_sum(x);
        y = s * s + 1;
        x = y;
        if (x == 65 || x == 122 || x == 26)
            return false;          /* absorbed by the cycle */
    } while (x >= 10);                /* loop until < 10 */

    return true;
}

seq_t a(idx_t n)
/* Return the nth term of the sequence A344208.  The function scans
   natural numbers in ascending order, counting those that satisfy the
   above property, and stops when the nth such number is found. */
{
    seq_t count = 0;
    seq_t k = 1;                       /* candidate numbers */

    while (1) {
        if (terminates(k)) {
            ++count;
            if (count == n)
                return k;
        }
        ++k;
    }
}
