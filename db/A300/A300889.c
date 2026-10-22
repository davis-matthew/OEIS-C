/*
 * A300889  a(n) is the least positive multiple of n which is a Fibbinary
 *          number (A003714).
 * https://oeis.org/A300889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least positive multiple of n that is a Fibbinary number
 *            (a binary representation with no consecutive 1’s). The function
 *            tests multiples of n until a Fibbinary number is found
 * Range:     u8 n <= 24, u16 n <= 380, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool */

/* Return true if x has no consecutive 1's in binary. */
static inline bool is_fibbinary(seq_t x)
{
    return (x & (x >> 1)) == 0;
}

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;          /* current multiple: k = 1 */
    for (idx_t k = 1; ; ++k) {
        if (is_fibbinary(m))
            return m;           /* m is the least Fibbinary multiple */
        m += (seq_t)n;          /* advance to the next multiple n·(k+1) */
    }
}
