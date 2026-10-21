/*
 * A243359  Number of steps it takes the terms in A029742 and their reversals
 *          to reach the value 9 when the smaller term is successively
 *          subtracted from the larger term.
 * https://oeis.org/A243359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243359: periodic with period 10.
   a(n) = t[n mod 10] where t[0]..t[9] =
   {5, 1, 1, 5, 3, 4, 2, 2, 4, 3} */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t table[10] = {5, 1, 1, 5, 3, 4, 2, 2, 4, 3};
    return table[(size_t)(n % 10)];
}
