/*
 * A360422  Numbers k such that k^2 + (sum of fourth powers of the digits of
 *          k^2) is a square.
 * https://oeis.org/A360422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref, fini, full, base
 * Formula:   numbers k such that k^2 + sum of fourth powers of digits of k^2
 *            is a square
 * Range:     u8 none, u16 none, u32 none, u64 n <= 4, u128 n <= 4, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes all such numbers up to k = 32918 once and caches them. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t seq[32919];
    static size_t term_count = 0;
    static bool initialized = false;

    if (!initialized)
    {
        initialized = true;
        const seq_t pow4[10] = {0, 1, 16, 81, 256, 625, 1296, 2401, 4096, 6561};

        for (idx_t k = 0; k <= 32918; ++k)
        {
            seq_t k_sq = (seq_t)k * k;
            seq_t tmp = k_sq;
            seq_t sum = 0;
            while (tmp > 0)
            {
                seq_t d = tmp % 10;
                sum += pow4[(int)d];
                tmp /= 10;
            }
            seq_t target = k_sq + sum;
            seq_t sqrt = floor_sqrt(target);
            if (sqrt * sqrt == target)
                seq[term_count++] = k;
        }
    }

    if (n == 0) return 0;            /* undefined for offset 1 */
    size_t idx = (size_t)(n - 1);
    return (idx < term_count) ? seq[idx] : 0;   /* zero if n is beyond the sequence */
}
