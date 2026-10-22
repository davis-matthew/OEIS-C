/*
 * A367595  List the positive integers but erase commas between terms if the
 *          digits just before and just after the comma have opposite parity.
 * https://oeis.org/A367595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = concatenate natural numbers, erasing a comma whenever the
 *            last digit of the previous number and the first digit of the
 *            next number have opposite parity
 * Range:     u8 none, u16 none, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation iterates natural numbers, builds each
   concatenated block until the parity condition fails, and
   returns the n‑th block.  No memoisation is required.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digits_of.h"   /* idx_t digits_of(idx_t x); */
#include "../utils/pow10.h"       /* seq_t pow10(idx_t e); */

/* return the most significant decimal digit of x   (x >= 1) */
static int first_digit(idx_t x)
{
    while (x >= 10)
        x /= 10;
    return (int)x;
}

/* a(n) – n‑th block in the sequence A367595 */
seq_t a(idx_t n)
{
    if (n < 1)            /* the problem assumes n>=1, but guard anyway */
        return (seq_t)0;

    idx_t block_idx = 0;          /* 0‑based counter of finished blocks */
    seq_t  current_block;
    idx_t  cur = 1;              /* current natural number being processed */

    while (block_idx < n) {
        /* Start a new block with the current number cur */
        current_block = (seq_t)cur;
        idx_t next = cur + 1;

        /* Attempt to merge following numbers while parity condition holds */
        while (1) {
            int last_digit = cur % 10;
            int first_next_digit = first_digit(next);

            if ((last_digit & 1) != (first_next_digit & 1)) {
                idx_t next_digits = digits_of(next);   /* number of decimal digits in next */
                current_block = current_block * pow10((idx_t)next_digits) + (seq_t)next;
                cur = next;
                next = cur + 1;
            } else {
                break;
            }
        }

        block_idx++;
        if (block_idx == n)
            return current_block;

        /* Prepare cur for the next block */
        cur = next;
    }

    /* control should never reach here */
    return (seq_t)0;
}
