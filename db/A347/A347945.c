/*
 * A347945  To get {a(n)}, start with the nonnegative integers sequence f()
 *          and, for each y>=0, shift the f(y) to position f(2y) and reset
 *          indices.
 * https://oeis.org/A347945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   uses the three explicit closed‑form expressions for A347945 from
 *            the OEIS entry: (1) a((3^t-3)/2) = (2^(t+2)+(-1)^(t-1)-9)/6 (2)
 *            a(k*3^t+(5*3^(t-1)-3)/2) = k*2^(t+1)+2^t+
 *            (2^(t+2)+(-1)^(t-1)-9)/6 (3) a(k*3^t+(7*3^(t-1)-3)/2) =
 *            k*2^(t+1)+2^t+ (2^(t+3)-(-1)^(t-1)-9)/6 The function associates
 *            each n...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   pow2_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2_128.h"

seq_t a(idx_t n)
{
    /* 3^t, start with t = 1 (i.e., 3^1 = 3) */
    __int128 pow3  = 3;
    int        t    = 1;

    while (1) {
        /* ----------- offset type (1) ------------- */
        __int128 offset0 = (pow3 - 3) / 2;          /* (3^t-3)/2 */
        if (offset0 == ( (__int128)n )) {
            int sign  = ((t-1) & 1) ? -1 : 1;      /* (-1)^(t-1) */
            long long tmp = (long long)pow2_128(t+2) + sign - 9;
            unsigned __int128 base = (unsigned __int128)(tmp/6);
            return (seq_t)base;                    /* a(n) = base */
        }

        /* ----------- offsets for type (2) and (3) ------------- */
        __int128 pow3_m1 = pow3 / 3;                /* 3^(t-1) */
        __int128 offset1 = (5*pow3_m1 - 3) / 2;     /* (5*3^(t-1)-3)/2 */
        if ((__int128)n >= offset1 &&
            ((__int128)n - offset1) % pow3 == 0) {
            idx_t k = (idx_t)(((__int128)n - offset1)/pow3);
            int sign  = ((t-1) & 1) ? -1 : 1;
            long long tmp = (long long)pow2_128(t+2) + sign - 9;
            unsigned __int128 base = (unsigned __int128)(tmp/6);
            unsigned __int128 term = ((unsigned __int128)k * pow2_128(t+1))
                                    + pow2_128(t)
                                    + base;
            return (seq_t)term;
        }

        __int128 offset2 = (7*pow3_m1 - 3) / 2;     /* (7*3^(t-1)-3)/2 */
        if ((__int128)n >= offset2 &&
            ((__int128)n - offset2) % pow3 == 0) {
            idx_t k = (idx_t)(((__int128)n - offset2)/pow3);
            int sign  = ((t-1) & 1) ? -1 : 1;
            long long tmp = (long long)pow2_128(t+3) - sign - 9;
            unsigned __int128 base = (unsigned __int128)(tmp/6);
            unsigned __int128 term = ((unsigned __int128)k * pow2_128(t+1))
                                    + pow2_128(t)
                                    + base;
            return (seq_t)term;
        }

        /* If offset0 already exceeded n, no higher t can work */
        if (offset0 > ( (__int128)n ))
            break;

        /* Advance to next t */
        pow3 *= 3;
        ++t;
    }

    /* Should never reach here for valid input */
    return (seq_t)0;
}
