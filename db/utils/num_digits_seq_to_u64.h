/* num_digits_seq_to_u64: the number of decimal digits of n (1 for 0).
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_NUM_DIGITS_SEQ_TO_U64_H
#define OEISC_UTILS_NUM_DIGITS_SEQ_TO_U64_H
#include <stddef.h>
#include <stdint.h>

/* helper: number of decimal digits of a seq_t value (non‑negative) */
static size_t num_digits_seq_to_u64(seq_t v)
{
    size_t len = 0;
    if (v == 0)
        return 1;
    while (v > 0)
    {
        ++len;
        v /= (seq_t)10;
    }
    return len;
}

#endif
