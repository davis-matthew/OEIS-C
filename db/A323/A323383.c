/*
 * A323383  Aliquot divisors of 24.
 * https://oeis.org/A323383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A323383: a(n) = 1,2,3,4,6,8,12 for 1 ≤ n ≤ 7, 0 otherwise. */
/* The sequence is finite; this implementation simply returns the known
   terms for the first seven indices and zero for all others. */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static array of the seven known terms */
    static const seq_t values[7] = { (seq_t)1, (seq_t)2, (seq_t)3,
                                     (seq_t)4, (seq_t)6, (seq_t)8,
                                     (seq_t)12 };

    if (n == 0)               /* safety guard for unexpected 0 input */
        return (seq_t)0;

    /* n is unsigned, so n-1 is safe after the guard above */
    if ((size_t)(n - 1) < 7)
        return values[n - 1];
    else
        return (seq_t)0;
}
