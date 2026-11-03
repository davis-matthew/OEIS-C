/* isqrt_u128_to_seq: floor(sqrt(n)).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_ISQRT_U128_TO_SEQ_H
#define OEISC_UTILS_ISQRT_U128_TO_SEQ_H
#include <stdint.h>

static inline seq_t isqrt_u128_to_seq(unsigned __int128 val)
{
    /* binary search for the integer square root of a 128‑bit number.
       The result always fits in 64 bits, because (2^64 - 1)^2 < 2^128. */
    unsigned __int128 low = 0;
    unsigned __int128 high = ((unsigned __int128)1 << 64) - 1;
    while (low <= high) {
        unsigned __int128 mid = low + (high - low) / 2;
        unsigned __int128 sq  = mid * mid;
        if (sq <= val) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return (seq_t)(high);
}

#endif
