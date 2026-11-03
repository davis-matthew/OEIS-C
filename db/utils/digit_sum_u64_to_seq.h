/* digit_sum_u64_to_seq: the sum of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_SUM_U64_TO_SEQ_H
#define OEISC_UTILS_DIGIT_SUM_U64_TO_SEQ_H
#include <stddef.h>
#include <stdint.h>

/* Sum of decimal digits of a 64‑bit integer.  The value is always
   small (≤ 2 digits), so the result fits easily in seq_t. */
static inline seq_t digit_sum_u64_to_seq(uint64_t v)
{
    seq_t s = 0;
    if (v == 0) return 0;
    while (v > 0) {
        s += v % 10;
        v /= 10;
    }
    return s;
}

#endif
