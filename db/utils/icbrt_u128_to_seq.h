/* icbrt_u128_to_seq: floor(cbrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ICBRT_U128_TO_SEQ_H
#define OEISC_UTILS_ICBRT_U128_TO_SEQ_H
#include <stdint.h>

/* Compute the integer cube root of an unsigned __int128 value.
   Returns the greatest integer r such that r^3 <= v. */
static seq_t icbrt_u128_to_seq(unsigned __int128 v)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = 1;

    /* Exponentially find an upper bound. */
    while (high * high * high <= v) {
        high <<= 1;
    }

    /* Binary search between low = high/2 and high. */
    low = high >> 1;
    while (low < high - 1) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 mid3 = mid * mid * mid;
        if (mid3 <= v) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return (seq_t)low;
}

#endif
