/*
 * A244681  The spiral of Champernowne read by the North ray.
 * https://oeis.org/A244681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ChampernowneDigit(4 n^2 – 9 n + 6)
 * Range:     u8 n <= 9, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ChampernowneDigit(pos) returns the pos‑th digit of the Champernowne constant
   0.1234567891011121314… (1–based index).  The sequence a(n) is the digits
   located on the north ray of a clockwise spiral filled with the digits of
   Champernowne.  The position of the n‑th term on that ray is
   pos(n) = 4 n^2 – 9 n + 6.  The digit is extracted by determining the
   length of the decimal block containing pos and then extracting the
   appropriate digit of the corresponding integer.  The implementation
   works for all builds supported by the test harness. */

#include <stdint.h>   /* for uint64_t, etc. */
#include <stddef.h>

static seq_t champernowne_digit(seq_t pos)
{
    /* Find the number of digits d of the block that contains pos. */
    seq_t pow10 = 1;          /* 10^(d-1) */
    seq_t cum = 0;            /* cumulative digits up to previous block */
    int    d   = 1;           /* current block length */

    while (1) {
        seq_t count = 9 * pow10;          /* numbers of d digits */
        seq_t block = count * d;          /* total digits in this block */
        if (cum + block >= pos) break;   /* pos lies in this block */
        cum += block;
        pow10 *= 10;
        ++d;
    }

    /* Position inside the block (1‑based). */
    seq_t offset   = pos - cum;           /* 1 ≤ offset ≤ d * count */
    seq_t num_idx  = (offset - 1) / d;    /* index of the number within the block */
    seq_t number   = pow10 + num_idx;     /* actual number (10^(d-1) + num_idx) */
    seq_t digit_idx= (offset - 1) % d;    /* 0 = most significant digit */

    /* Extract the digit. */
    seq_t pow = 1;
    for (int i = 0; i < d - digit_idx - 1; ++i) {
        pow *= 10;
    }
    return (number / pow) % 10;
}

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;
    seq_t pos  = 4 * nn * nn - 9 * nn + 6;   /* position on the north ray */
    return champernowne_digit(pos);
}
