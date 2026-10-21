/*
 * A257079  The least nonzero digit missing from the factorial representation
 *          (A007623) of n.
 * https://oeis.org/A257079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9559, u16 n <= 9559, u32 n <= 9559, u64 n <= 9559,
 *            u128 n <= 9559, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Least nonzero digit missing from the factorial representation of n
 *  (OEIS A257079).  The function computes the factoradic digits of n
 *  using the standard algorithm:
 *
 *      for i = 1,2,...:
 *          digit_i = n mod (i+1)
 *          n      = n / (i+1)
 *
 *  The digits satisfy 0 ≤ digit_i ≤ i.  The result is the smallest
 *  positive integer that does not occur among those digits (0 is
 *  ignored).  This works for any n ≥ 0 and has O(log n) time for
 *  the factorial base conversion.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)           /* 0! representation is just "0"            */
        return (seq_t)1;  /* 1 is the smallest missed positive digit   */

    idx_t  m = n;         /* copy of input, will be divided            */
    uint64_t mask = 0;    /* bit i set if digit i appears (i≥1)       */
    idx_t i = 1;          /* current factorial index (i+1 used below)   */

    while (m > 0) {
        idx_t digit = m % (i + 1);     /* current digit in position i   */
        m /= (i + 1);                  /* prepare for next iteration   */
        if (digit > 0)                 /* skip the digit 0              */
            mask |= ((uint64_t)1 << digit);
        ++i;
    }

    /* find the smallest positive integer not present in the set of digits */
    idx_t missing = 1;
    while (mask & ((uint64_t)1 << missing))
        ++missing;

    return (seq_t)missing;
}
