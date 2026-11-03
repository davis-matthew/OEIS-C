/*
 * A253600  Smallest exponent k>1 such that n and n^k have some digits in
 *          common.
 * https://oeis.org/A253600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9538 (2026-11-01)
 * Helpers:   digit_counts, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A253600 – smallest exponent k > 1 such that
 *  n and n^k have a common decimal digit.
 *  In practice k ≤ 5 for all n, so we search k = 2 … 5. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/digit_counts.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Digit set of the original number n. */
    unsigned char freq_n[10];
    digit_counts((seq_t)n, freq_n);
    unsigned mask_n = 0;
    for (int d = 0; d < 10; ++d)
        if (freq_n[d]) mask_n |= 1u << d;

    /* Search for the smallest k > 1 that shares a digit with n. */
    for (idx_t k = 2; k <= 5; ++k) {
        seq_t pow = ipow((seq_t)n, (seq_t)k);

        unsigned char freq_p[10];
        digit_counts(pow, freq_p);
        unsigned mask_p = 0;
        for (int d = 0; d < 10; ++d)
            if (freq_p[d]) mask_p |= 1u << d;

        if (mask_n & mask_p)
            return (seq_t)k;
    }

    /* According to the theory this line is unreachable,
     * but provide a safe default in case of unexpected input. */
    return (seq_t)6;
}
