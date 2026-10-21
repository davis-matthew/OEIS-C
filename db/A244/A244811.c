/*
 * A244811  The hexagonal spiral of Champernowne, read along the 330-degree
 *          ray.
 * https://oeis.org/A244811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = digit at position k = 3*n*n - 6*n + 4 in Champernowne's *
 *            concatenation 123456789101112… . * The implementation extracts
 *            the required digit without using any * floating‑point or
 *            large‑integer libraries. It works for every n ≥ 1 * and for all
 *            supported seq_t types
 * Range:     u8 n <= 15, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* needed for bool if used in helper prototypes */

/* Return the digit (0‑9) at a 1‑based position pos in Champernowne’s constant. */
static seq_t champernowne_digit(idx_t pos)
{
    idx_t idx   = pos;                 /* 1‑based index inside the Champernowne string */
    idx_t d     = 1;                   /* current digit length of the numbers */
    idx_t start = 1;                   /* first integer with digit length d */

    /* Find the block that contains the requested position. */
    while (1) {
        /* Total number of digits contributed by all d‑digit numbers. */
        idx_t count = 9 * start * d;
        if (idx <= count) break;
        idx   -= count;   /* skip this whole block */
        d++;             /* move to the next block */
        start *= 10;     /* shift to the next magnitude of numbers */
    }

    /* idx now points to a position inside the d‑digit block. */
    idx_t offset      = (idx - 1) / d;     /* 0‑based index of the desired number */
    idx_t digit_index = (idx - 1) % d;     /* 0‑based index of the digit inside that number */

    seq_t number = (seq_t)start + (seq_t)offset;   /* the actual number that contains the digit */

    /* Extract the digit.  For a d‑digit number, the most significant
     * digit corresponds to digit_index = 0.  We compute the factor
     * 10^(d-1-digit_index) and divide before taking the last digit. */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < d - 1 - digit_index; ++i)
        pow10 *= 10;          /* seq_t multiplication works for all supported types */

    return (number / pow10) % 10;   /* the required digit */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    /* k = 3*n^2 - 6*n + 4  (1‑based position in Champernowne's constant). */
    idx_t k = 3 * n * n - 6 * n + 4;
    return champernowne_digit(k);
}
