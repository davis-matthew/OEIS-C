/* digit_sum_idx_to_seq: the sum of the decimal digits of n.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_DIGIT_SUM_IDX_TO_SEQ_H
#define OEISC_UTILS_DIGIT_SUM_IDX_TO_SEQ_H
#include <stdint.h>
#include <stdlib.h>

/* Local helper: compute sum of decimal digits of an idx_t value. */
static seq_t digit_sum_idx_to_seq(idx_t x)
{
    seq_t sum = 0;
    while (x) {
        sum += (seq_t)(x % 10);
        x /= 10;
    }
    return sum;
}

#endif
