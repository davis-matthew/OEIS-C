/*
 * A286651  Decimal expansion of 1/824633702441.
 * https://oeis.org/A286651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth decimal digit of 1/824633702441 using long division. */

#include <stdint.h>

seq_t a(idx_t n)
{
    const uint64_t DEN = 824633702441ULL;   /* denominator */
    uint64_t rem = 1;                      /* initial remainder 1/denominator */
    seq_t digit = 0;

    for (idx_t i = 0; i <= n; ++i)
    {
        uint64_t prod = rem * 10ULL;       /* multiply remainder by 10 */
        digit = (seq_t)(prod / DEN);       /* quotient is the digit */
        rem  = prod % DEN;                 /* new remainder for next step */
    }

    return digit;                          /* digit for position n */
}
