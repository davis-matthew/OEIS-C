/*
 * A265026  First differences of A048701.
 * https://oeis.org/A265026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A048701(n) - A048701(n-1) A048701(n) is the n-th binary
 *            palindrome of even length (sorted by value). a(1) = 3 (the first
 *            such palindrome)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 32767, u64 n <= 32767,
 *            u128 n <= 32767, big any n
 * Validated: standard, pass, 82 terms through n = 8039 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the n-th binary palindrome of even length.
   The sequence starts at n = 1. */
static seq_t nth_pal_even(idx_t n)
{
    /* Determine the block length.
       Let k be the smallest integer such that 2^k - 1 >= n.
       Then the palindrome has length 2k bits.
       2^k >= n+1 .
    */
    idx_t k = 0;
    /* Use a loop to avoid relying on intrinsics that may not exist. */
    while (( (idx_t)1 << k ) < n + 1) ++k;

    /* Compute offset within the block (0‑based). */
    idx_t offset = n - ( (idx_t)1 << (k - 1) );

    /* Build the left half (k bits) : first bit is 1, remaining k-1 bits
       are the binary representation of 'offset' (padded with leading zeros). */
    seq_t left = ((seq_t)1 << (k - 1)) | ((seq_t)offset);

    /* Reverse the k bits of 'left' to form the right half. */
    seq_t rev = 0;
    for (idx_t i = 0; i < k; ++i)
    {
        if ( (left >> i) & 1 )
            rev |= ((seq_t)1 << (k - 1 - i));
    }

    /* Concatenate left and reversed bits to get the full even‑length
       binary palindrome. */
    return (left << k) | rev;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1, return 0 as safe value */

    if (n == 1)
        return nth_pal_even(1);   /* a(1) = 3 */

    seq_t cur  = nth_pal_even(n);
    seq_t prev = nth_pal_even(n - 1);
    return cur - prev;
}
