/*
 * A340836  a(n) is the least k such that the binary reversal of k is greater
 *          than or equal to n.
 * https://oeis.org/A340836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that the binary reversal of k is >= n
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute the binary reversal of x.  The reversal uses only the bits
 * up to the most significant set bit,ignoring leading zeros after
 * the reversal. */
static inline seq_t rev_bits(seq_t x)
{
    seq_t rev = 0;
    while (x)
    {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;   /* cast the index to seq_t for comparison */
    seq_t k;

    /* Property of the sequence: a(n) ≤ n + 1.  Therefore
     * only k up to target+1 need to be examined. */
    for (k = 0; k <= target + 1; ++k)
    {
        if (rev_bits(k) >= target)
            return k;
    }

    /* The loop should always return before this point; the fallback
     * ensures a valid return value in extreme edge cases. */
    return target + 1;
}
