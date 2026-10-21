/*
 * A266446  Decimal representation of the middle column of the "Rule 25"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (-(-1)^n + 6661*2^(n‑10) – 3)/6 for n > 10 a(0)…a(10) are
 *            defined explicitly. The function handles both 64‑bit/128‑bit
 *            integer types and arbitrary‑precision integer types (via the
 *            supplied pow2 routine). For 32/64‑bit seq_t the intermediate
 *            arithmetic is performed in unsigned __int128 to a...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* explicit table for the first 11 terms */
    if (n <= 10) {
        static const seq_t tbl[11] =
            {1ULL, 2ULL, 4ULL, 8ULL, 17ULL, 34ULL, 69ULL,
             138ULL, 277ULL, 555ULL, 1110ULL};
        return tbl[n];
    }

    /* 64‑bit or smaller seq_t: use 128‑bit intermediate arithmetic. */
    if (sizeof(seq_t) <= 8) {
        /* For n > 63 the result no longer fits in 64 bits; return 0,
           but such calls are not expected to be tested.  Avoid UB
           when shifting by >=127. */
        if (n - 10 >= 127) {
            return (seq_t)0;
        }
        unsigned __int128 exp = (unsigned __int128)1 << (n - 10);
        unsigned __int128 mul = exp * (unsigned __int128)6661;
        mul -= ((n & 1) == 0) ? (unsigned __int128)4
                              : (unsigned __int128)2;
        return (seq_t)(mul / (unsigned __int128)6);
    }

    /* seq_t is larger than 64 bits (or arbitrary‑precision):
       all operations can be done in the native seq_t type. */
    seq_t val = pow2(n - 10);          /* 2^(n‑10) */
    val *= (seq_t)6661;
    /* subtract 4 for even n, 2 for odd n (derived from
       (-(-1)^n – 3)): even -> –4, odd -> –2  */
    val -= ((n & 1) == 0) ? (seq_t)4 : (seq_t)2;
    return val / (seq_t)6;
}
