/*
 * A266841  Binary representation of the n-th iteration of the "Rule 69"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A266841: Binary representation of the n-th iteration of Rule 69
   starting with a single ON cell.

   Formula (Conjecture from Colin Barker):
     a(0) = 1
     a(n) = (10 * 100^n + 100^⌊n/2⌋ - 11) / 99      for odd n
     a(n) = (100 * 100^n - 10^n) / 99                for even n
*/

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if (n & 1) {                /* odd n */
        seq_t pow100_n = seq_pow((seq_t)100, n);
        seq_t pow100_half = seq_pow((seq_t)100, n / 2);

        /* (10*100^n + 100^floor(n/2) - 11) / 99 */
        return (((seq_t)10 * pow100_n) + pow100_half - (seq_t)11) / (seq_t)99;
    } else {                     /* even n */
        seq_t pow100_n = seq_pow((seq_t)100, n);
        seq_t pow10_n = seq_pow((seq_t)10, n);

        /* (100*100^n - 10^n) / 99 */
        return (((seq_t)100 * pow100_n) - pow10_n) / (seq_t)99;
    }
}
