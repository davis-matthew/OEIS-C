/*
 * A358241  Number of connected Dynkin diagrams with n nodes.
 * https://oeis.org/A358241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2)=3; a(3)=3; a(4)=5; a(5)=4; a(6)=5; a(7)=5; a(8)=5;
 *            a(n)=4 for all n>=9. Connected Dynkin diagrams by number of
 *            nodes
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n == 0, but return 0 to keep the function defined. */
    if (n == 0) return (seq_t)0;

    if (n == 1)                return (seq_t)1;
    if (n == 2 || n == 3)     return (seq_t)3;
    if (n == 4 || n == 6 || n == 7 || n == 8) return (seq_t)5;
    /* For n = 5 and all n >= 9 */
    return (seq_t)4;
}
