/*
 * A244814  The hexagonal spiral of Champernowne, read along the 240-degree
 *          ray.
 * https://oeis.org/A244814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the decimal digit at position 12 n² – 21 n + 10 in
 *            Champernowne's concatenation of the natural numbers
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function locates the block of equal‑length numbers containing that position, and extracts the appropriate digit. */

#include <stdint.h>

static uint64_t pow10_uint64(uint64_t e)
{
    uint64_t r = 1;
    for (uint64_t i = 0; i < e; ++i)
        r *= 10ULL;
    return r;
}

seq_t a(idx_t n)
{
    /* position in Champernowne string (1‑based) */
    uint64_t pos = 12ULL * (uint64_t)n * (uint64_t)n
                    - 21ULL * (uint64_t)n
                    + 10ULL;

    /* iterate over digit blocks */
    uint64_t digit_len  = 1;        /* number of digits of the current block */
    uint64_t start      = 1;        /* first number of the current block (10^(digit_len-1)) */
    uint64_t block_cnt  = 9;        /* how many numbers in the current block */
    uint64_t cumulative = 0;        /* total digits processed so far */

    while (1)
    {
        uint64_t block_digits = digit_len * block_cnt;

        if (pos <= cumulative + block_digits)
        {
            /* desired digit lies inside this block */
            uint64_t offset       = pos - cumulative - 1;          /* 0‑based offset within the block */
            uint64_t number_off   = offset / digit_len;            /* how many numbers after 'start' */
            uint64_t digit_index  = offset % digit_len;            /* 0‑based position inside the number */

            uint64_t number = start + number_off;

            /* Extract the digit at position digit_index from the left.  */
            uint64_t pow = 1;
            for (uint64_t i = 0; i < digit_len - digit_index - 1; ++i)
                pow *= 10ULL;

            uint64_t digit = (number / pow) % 10ULL;
            return (seq_t)digit;
        }

        cumulative += block_digits;
        /* advance to next block: numbers with one more decimal digit */
        digit_len++;
        start *= 10ULL;
        block_cnt = 9ULL * start;
    }
}
