/*
 * A381946  a(n) is the smallest positive integer k with at least one digit >
 *          1 such that k*n contains all the distinct digits of n.
 * https://oeis.org/A381946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A381946:  a(n) = smallest positive integer k (k has a digit > 1)
 * such that the decimal digits of n all appear in the decimal representation of k*n. */

/* The utilities in the build provide digit_counts and all required types.
 * We include the header that declares digit_counts. */
#include <stdint.h>
#include "../utils/digit_counts.h"

/* Return a 10‑bit mask describing which decimal digits appear in x. */
static uint16_t mask_from_seq(seq_t x)
{
    unsigned char cnt[10] = {0};
    digit_counts(x, cnt);
    uint16_t mask = 0;
    for (int d = 0; d < 10; ++d) {
        if (cnt[d]) mask |= 1u << d;
    }
    return mask;
}

/* Compute a(n) for the sequence A381946. */
seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;              /* value of n in seq_t */
    uint16_t target = mask_from_seq(sn); /* digits that must appear in k*n */

    for (idx_t k = 1; ; ++k) {
        /* k must contain at least one digit greater than 1 */
        uint16_t maskk = mask_from_seq((seq_t)k);
        if ((maskk & (~0x3u)) == 0)  /* bits 0 and 1 only, i.e. digits 0 or 1 only */
            continue;

        seq_t prod = (seq_t)k * sn;   /* k*n */
        uint16_t maskp = mask_from_seq(prod);

        /* All digits of n must be present in k*n */
        if ((maskp & target) == target)
            return (seq_t)k;
    }
}
