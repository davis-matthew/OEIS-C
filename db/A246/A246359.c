/*
 * A246359  Maximum digit in the factorial base expansion of n (A007623).
 * https://oeis.org/A246359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum digit in factorial base expansion of n with a(0)
 *            = 0
 * Range:     u8 n <= 2390, u16 n <= 9693, u32 n <= 9693, u64 n <= 9693,
 *            u128 n <= 9693, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t maxdigit = 0;
    if (n == 0) return 0;
    seq_t n_val = (seq_t)n;
    int i = 1;
    while (n_val > 0)
    {
        seq_t div = (seq_t)(i + 1);
        seq_t digit = n_val % div;
        if (digit > maxdigit) maxdigit = digit;
        n_val /= div;
        ++i;
    }
    return maxdigit;
}
