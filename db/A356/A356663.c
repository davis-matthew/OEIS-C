/*
 * A356663  Number of ways to create an angle excess of n degrees using 3
 *          distinct regular polygons with integral internal angles.
 * https://oeis.org/A356663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches integer triples of distinct divisors of 360 and counts angle excess */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* table of counts for angle excess 1 … 174; 0 otherwise */
    static seq_t counts[175] = {0};
    static bool initialized = false;

    if (!initialized) {
        /* all divisors of 360 greater than 2 (the possible numbers of sides) */
        const uint32_t divisors[22] = {
            3, 4, 5, 6, 8, 9, 10, 12,
            15, 18, 20, 24, 30, 36, 40, 45,
            60, 72, 90, 120, 180, 360
        };
        const size_t D = 22;

        for (size_t i = 0; i < D; ++i)
            for (size_t j = i + 1; j < D; ++j)
                for (size_t k = j + 1; k < D; ++k) {
                    /* interior angles (integers) */
                    idx_t ai = 180 - 360 / divisors[i];
                    idx_t bi = 180 - 360 / divisors[j];
                    idx_t ci = 180 - 360 / divisors[k];
                    idx_t sum = ai + bi + ci;          /* 3*180 - 360*(1/a+1/b+1/c) */
                    idx_t excess = sum - 360;          /* n = sum - 360 */
                    if (excess > 0 && excess <= 174u)
                        counts[excess] = (seq_t)(counts[excess] + 1);
                }

        initialized = true;
    }

    if (n <= 174u)
        return counts[n];
    return 0;
}
