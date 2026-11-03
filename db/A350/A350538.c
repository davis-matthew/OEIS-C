/*
 * A350538  a(n) is the smallest proper multiple of n which contains only even
 *          digits.
 * https://oeis.org/A350538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 98, u32 n <= 9998, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9944 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Test whether all decimal digits of x are even. */
static inline bool all_even_digits(seq_t x)
{
    if (x == 0)           /* 0 is considered to have only even digits */
        return true;

    while (x != 0) {
        seq_t dig   = x % 10;     /* least significant decimal digit   */
        if ((dig % 2) != 0)       /* odd digit found                    */
            return false;
        x /= 10;                  /* move to next digit                  */
    }
    return true;
}

/* Computes the smallest proper multiple of n that contains only even
   decimal digits (search). */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast to seq_t for safe arithmetic   */
    seq_t cand = nn * 2;          /* first proper multiple (k = 2)       */

    while (!all_even_digits(cand)) {
        cand += nn;               /* next multiple: (k+1)*n             */
    }
    return cand;
}
