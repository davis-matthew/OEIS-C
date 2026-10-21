/*
 * A275993  Champernowne sequence: write n in base 16 and juxtapose.
 * https://oeis.org/A275993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) gives the hexadecimal digit (converted to decimal) at *
 *            position n in the Champernowne sequence obtained by writing the
 *            * non‑negative integers in base 16 one after another
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow16_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation walks through blocks of numbers that have the same
 * number of hex digits, finds the block in which n lies, then extracts
 * the appropriate digit. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow16_u64.h"

/* Main function */
seq_t a(idx_t n)
{
    /* Numbers 0 .. 15 produce one hex digit each. */
    if (n < 16)
        return (seq_t)n;

    /* Remaining position after the initial 16 digits. */
    idx_t rem = n - 16;
    int len = 2;          /* current block length in hex digits */

    while (1)
    {
        /* Number of digits contributed by this block. */
        uint64_t start  = pow16_u64(len - 1);           /* 16^(len‑1) */
        uint64_t count  = 15ULL * start;                /* numbers in block */
        /* Number of the position within this block (0‑based). */
        uint64_t num_in_block = rem / len;              /* index of the number */
        int      pos_in_num  = rem % len;               /* digit position from left */

        if (num_in_block < count)
        {
            /* This is the block that contains the desired digit. */
            /* Compute divisor 16^(len‑1‑pos_in_num). */
            uint64_t divisor = pow16_u64(len - 1 - pos_in_num);
            /* The actual number whose digit we need. */
            uint64_t number = start + num_in_block;
            /* Extract the digit: ((number / divisor) % 16). */
            uint64_t digit  = (number / divisor) % 16ULL;
            return (seq_t)digit;   /* value is always 0..15 */
        }
        else
        {
            /* Skip this entire block.  For len <= 14 the product fits in
             * uint64_t; for len >= 15 the block is large enough that
             * we would have broken above.  So this subtraction never
             * overflows on uint64_t. */
            uint64_t block_digits = count * (uint64_t)len;
            rem -= block_digits;
            ++len;
        }
    }
}
