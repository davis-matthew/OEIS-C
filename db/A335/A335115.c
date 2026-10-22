/*
 * A335115  a(2*n) = 2*n - a(n), a(2*n+1) = 2*n + 1.
 * https://oeis.org/A335115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335115
   a(n) = n                      if n is odd
   a(n) = n - a(n/2)             if n is even
   a(1) = 1 (base case)
   This recurrence is equivalent to the definition
   a(2*n) = 2*n - a(n), a(2*n+1) = 2*n+1.
*/

#include <stdint.h> /* for uint64_t, etc.  (included for safety) */

seq_t a(idx_t n)
{
    if (n == 1)                         /* base case */
        return (seq_t)1;
    if (n & 1)                          /* n is odd */
        return (seq_t)n;
    /* n is even: compute a(n/2) recursively */
    seq_t half = a(n >> 1);              /* n/2 */
    return (seq_t)n - half;              /* n - a(n/2) */
}
