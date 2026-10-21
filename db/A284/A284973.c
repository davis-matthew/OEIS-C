/*
 * A284973  Numbers with digits 4 and 9 only.
 * https://oeis.org/A284973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers composed only of digits 4 and 9 in increasing order. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the minimal length L of the numbers in the sequence such that
       the total count of all numbers with length <= L is at least n.
       The total count for numbers of length < L is stored in 'total'. */
    uint64_t total = 0;          /* cumulative count for lengths < L */
    size_t   L = 1;              /* current length */

    for (;;)
    {
        uint64_t count = 1ULL << L;            /* 2^L numbers of this length */
        if (total + count >= n)                /* n lies within this block */
            break;
        total += count;
        ++L;
    }

    /* Offset within the block (0‑based). */
    uint64_t offset = n - total - 1ULL;

    /* Build the decimal number digit by digit, most significant first. */
    seq_t result = 0;
    for (size_t i = L; i > 0; --i)
    {
        seq_t digit = ((offset >> (i - 1)) & 1ULL) ? (seq_t)9 : (seq_t)4;
        result = result * 10 + digit;
    }

    return result;
}
