/*
 * A328447  Smallest representative of the class of numbers having the same
 *          digits as n up to permutation.
 * https://oeis.org/A328447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest representative of the class of numbers having the same digits as n
 * up to permutation: sort the digits in increasing order.  If the leading
 * digit is 0, move the smallest non‑zero digit to the front.  The function
 * implements this definition directly by counting digit frequencies. */
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* Count digits of n.  digit_frequencies accepts a seq_t value. */
    seq_t x = (seq_t)n;
    unsigned int freq[10];
    digit_frequencies(x, freq);

    /* Special case: n == 0. */
    if (n == 0u)
        return (seq_t)0;

    /* Find the smallest non‑zero digit present. */
    int first_nonzero = 0;
    for (int d = 1; d <= 9; ++d)
    {
        if (freq[d])
        {
            first_nonzero = d;
            break;
        }
    }

    /* Construct the smallest number by appending digits in order:
     * 1. the smallest non‑zero digit,
     * 2. all zeros,
     * 3. the remaining digits in ascending order. */
    seq_t result = (seq_t)first_nonzero;
    freq[first_nonzero]--;   /* used one instance of this digit */

    /* Append zeros immediately after the first non-zero digit. */
    for (unsigned int i = 0; i < freq[0]; ++i)
        result = result * 10;  /* +0 is implicit */

    /* Append remaining digits in ascending order. */
    for (int d = 1; d <= 9; ++d)
    {
        while (freq[d] > 0)
        {
            result = result * 10 + (seq_t)d;
            --freq[d];
        }
    }

    return result;
}
