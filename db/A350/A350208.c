/*
 * A350208  Given the sequence of the digits of the Champernowne constant
 *          A033307 we start with number 1 and advance as many digits as the
 *          number that it indicates. If the digit is 0 then advance 10
 *          positions.
 * https://oeis.org/A350208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Recurrence:
 *   a(1) = 1
 *   For n > 1:
 *       step    = (a(n-1) == 0) ? 10 : a(n-1)
 *       a(n)    = digit_at( pos_{n-1} + step )
 *   where pos1 = 1 and pos_k is the 1‑based position in Champernowne's
 *   constant from which a(k) is read.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* powers of 10 up to 10^19 – sufficient for any n encountered here */
static const idx_t pow10[20] = {
    1ULL,
    10ULL,
    100ULL,
    1000ULL,
    10000ULL,
    100000ULL,
    1000000ULL,
    10000000ULL,
    100000000ULL,
    1000000000ULL,
    10000000000ULL,
    100000000000ULL,
    1000000000000ULL,
    10000000000000ULL,
    100000000000000ULL,
    1000000000000000ULL,
    10000000000000000ULL,
    100000000000000000ULL,
    1000000000000000000ULL
};

/* Return the decimal digit (0‑9) that occupies position `pos`
 * (1‑based) in Champernowne's constant. */
static seq_t digit_at(idx_t pos)
{
    idx_t d = 1;                      /* current block has `d`‑digit numbers */
    while (1)
    {
        idx_t   block_count = 9ULL * pow10[d - 1];              /* number of numbers */
        __int128 block_digits = (__int128)block_count * d;     /* digits in the block */

        if ((__int128)pos > block_digits) {
            pos -= (idx_t)block_digits;
            ++d;
            continue;
        }

        /* position lies within this block */
        idx_t offset        = pos - 1;                         /* 0‑based within block */
        idx_t num_index     = offset / d;                      /* which number */
        idx_t digit_index   = offset % d;                      /* which digit of number */
        idx_t number        = pow10[d - 1] + num_index;        /* actual number */
        idx_t divisor       = pow10[d - 1 - digit_index];      /* 10^k to isolate digit */

        return (seq_t)((number / divisor) % 10ULL);
    }
}

/* Main sequence function.  It works for any n >= 1; calls in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n < 1 */

    idx_t  pos    = 1;                    /* position of the current digit */
    seq_t  digit  = 1;                    /* a(1) */

    if (n == 1) return digit;

    for (idx_t i = 2; i <= n; ++i) {
        idx_t step = (digit == 0) ? 10 : (idx_t)digit;
        pos += step;
        digit = digit_at(pos);
    }
    return digit;
}
