/*
 * A285383  Limiting 0-word of the morphism 0 -> 11, 1 -> 01.
 * https://oeis.org/A285383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) from morphism 0 -> 11, 1 -> 01
   Recurrence:
        a(1) = 0
        a(2k) = 1
        a(2k-1) = 1 - a(k)
   (k = (n+1)/2 for odd n)
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if ((n & 1ULL) == 0) return 1;
    return 1 - a((n + 1) >> 1);
}
