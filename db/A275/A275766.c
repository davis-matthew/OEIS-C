/*
 * A275766  a(n) = (5^(2*(n + 1)) - 1)/4.
 * https://oeis.org/A275766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^(2*(n + 1)) - 1)/4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* 5^(2*(n+1)) = 25^(n+1) */
    if (sizeof(seq_t) <= 8) {          /* 64‑bit or smaller builds            */
        unsigned __int128 res = 1;
        for (idx_t i = 0; i < n + 1; ++i)
            res *= 25;                  /* 25^(n+1) */
        res = (res - 1) / 4;            /* (25^(n+1) - 1) / 4                */
        return (seq_t)res;              /* cast to seq_t – truncation OK for
                                            values that fit in seq_t            */
    } else {                            /* arbitrary‑precision builds           */
        seq_t res = seq_pow((seq_t)25, (idx_t)(n + 1)); /* 25^(n+1)      */
        res = (res - 1) / 4;                          /* exact division   */
        return res;                                    /* full precision   */
    }
}
