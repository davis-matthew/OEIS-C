/*
 * A244691  The spiral of Champernowne read by the Southeast ray.
 * https://oeis.org/A244691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit of Champernowne constant at position (2*n - 1)^2
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute 10^e as idx_t (e up to 9 for n up to ~10^4) */
static idx_t pow10_idx(idx_t e)
{
    idx_t r = 1;
    while (e--) r *= 10;
    return r;
}

seq_t a(idx_t n)
{
    /* position in the Champernowne string */
    idx_t first = 2 * n - 1;
    idx_t pos   = first * first;                /* (2n-1)^2 */

    idx_t digits_processed = 0;                  /* digits seen so far */
    idx_t len  = 1;                              /* current number length */
    idx_t cnt  = 9;                               /* count of numbers of this length */

    for (;;)
    {
        idx_t block_digits = cnt * len;          /* digits contributed by this block */

        if (digits_processed + block_digits >= pos)
        {
            /* the desired digit lies in this block */
            idx_t offset      = pos - digits_processed - 1;   /* 0‑based within block */
            idx_t number_index = offset / len;                /* 0‑based index of number */
            idx_t digit_index  = offset % len;                /* 0‑based digit position */

            idx_t number  = pow10_idx(len - 1) + number_index;
            idx_t divisor = pow10_idx(len - 1 - digit_index);

            seq_t digit = (seq_t)((number / divisor) % 10);
            return digit;
        }

        /* move to the next length block */
        digits_processed += block_digits;
        len++;
        cnt *= 10;
    }
}
