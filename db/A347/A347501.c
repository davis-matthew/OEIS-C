/*
 * A347501  Number of dominating sets in the n-alkane graph.
 * https://oeis.org/A347501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 17; for n > 1, a(n) = 81 * 5^(n-2)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utilities needed for the calculation */
#include <stdint.h>
#include <stdbool.h>

static inline seq_t power5(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Handle the offset explicitly */
    if (n == 1)
        return 17;

    /* Compute 5^(n-2) using the provided power5 helper */
    seq_t pow5 = power5(n - 2);

    /* Scale by 81 to get the final result */
    return 81 * pow5;
}
