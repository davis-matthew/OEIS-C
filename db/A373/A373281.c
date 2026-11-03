/*
 * A373281  Expansion of Sum_{k>=0} x^(5^k) / (1 - 5*x^(5^k)).
 * https://oeis.org/A373281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^(n-1) + a(n/5) if n mod 5 == 0; otherwise a(n) =
 *            5^(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 963 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided power5 utility for efficient 5^exp computation. */

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
    if (n == 0) /* Undefined for n=0, but not used; return 0 to avoid UB. */
        return 0;

    seq_t result = power5(n - 1);

    if ((n % 5) == 0)
        result += a(n / 5);

    return result;
}
