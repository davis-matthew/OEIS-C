/*
 * A296744  Numbers whose base-11 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          = #(falls); see Comments.
 * https://oeis.org/A296744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the nth term of OEIS sequence A296744.
   The sequence consists of natural numbers whose base‑11 representation
   has the same number of rises (a digit < its successor) and falls
   (a digit > its successor).  For a given n (1‑based index) this
   function returns the corresponding term a(n) by brute‑force search.
   Offset: 1 (a(1) = 1)
*/

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: first term is 1 */
    if (n < 1) return 0;

    seq_t count = 0;
    for (seq_t x = 1;; ++x)
    {
        /* Extract base‑11 digits into array (least significant first) */
        seq_t tmp = x;
        unsigned digits[128];
        unsigned len = 0;
        while (tmp > 0)
        {
            digits[len++] = (unsigned)(tmp % 11);
            tmp      /= 11;
        }
        /* Numbers with a single digit automatically satisfy the condition */
        if (len <= 1)
        {
            count++;
            if (count == n) return x;
            continue;
        }

        /* Count rises and falls in most significant → least significant order */
        int rises = 0, falls = 0;
        /* digits are stored lsb→msb, so traverse from msb downwards */
        for (unsigned j = len - 1; j > 0; --j)
        {
            if (digits[j] < digits[j - 1]) ++rises;
            else if (digits[j] > digits[j - 1]) ++falls;
        }

        if (rises == falls)
        {
            count++;
            if (count == n) return x;
        }
    }
}
