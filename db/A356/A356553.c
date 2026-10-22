/*
 * A356553  For any n > 0, let b > 1 be the least base where the sum of digits
 *          of n divides n; a(n) is the sum of digits of n in base b.
 * https://oeis.org/A356553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of the digits of n in the least base b>1 such that *
 *            the digit sum divides n. (OEIS A356553)
 * Range:     u8 n <= 1556, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sum of the digits of n in base b. */
static seq_t digit_sum_base(seq_t n, seq_t b)
{
    seq_t sum = 0;
    while (n)
    {
        sum += n % b;
        n   /= b;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n > 0.  A guard for n == 0 is
     * included for robustness, even though the OEIS offset starts at 1. */
    if (n == 0) return 0;

    /* Handle n == 1 separately because the search loop (b <= n) would
     * otherwise skip the necessary base b = 2. */
    if (n == 1) return 1;

    seq_t nn = (seq_t)n;
    for (idx_t b = 2; b <= n; ++b)
    {
        seq_t sum = digit_sum_base(nn, (seq_t)b);
        /* sum > 0 because n > 0 and b > 1 */
        if (nn % sum == 0)
            return sum;
    }

    /* Theoretically unreachable: for any n > 0 the base b = n yields
     * digit sum 1, which always divides n.  Returning 1 as a safety. */
    return 1;
}
