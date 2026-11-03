/* digit_sum_u64_to_u32: the sum of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_SUM_U64_TO_U32_H
#define OEISC_UTILS_DIGIT_SUM_U64_TO_U32_H
#include <stdint.h>

/* Compute the decimal digit sum of a uint64_t value. */
static inline unsigned digit_sum_u64_to_u32(uint64_t x)
{
    unsigned sum = 0;
    while (x != (uint64_t)0)
    {
        sum += (unsigned)(x % (uint64_t)10);
        x /= (uint64_t)10;
    }
    return sum;
}

#endif
