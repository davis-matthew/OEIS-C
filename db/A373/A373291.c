/*
 * A373291  Least perfect power of n containing some decimal digit of n.
 * https://oeis.org/A373291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   digit_counts, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = smallest perfect power n^k (k>1) such that n^k contains
   at least one decimal digit of n. This is done by brute‑forcing k and
   testing digit overlap. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/digit_counts.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    unsigned char cntN[10] = {0};
    /* Count digits of n */
    digit_counts((seq_t)n, cntN);

    for (idx_t k = 2; ; ++k)
    {
        /* Compute n^k */
        seq_t power = ipow_u64_idx((uint64_t)n, k);

        /* Count digits of the power */
        unsigned char cntP[10];
        digit_counts(power, cntP);

        /* Check for any common digit */
        for (int d = 0; d < 10; ++d)
            if (cntN[d] && cntP[d])
                return power;
    }
}
