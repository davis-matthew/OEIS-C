/*
 * A353437  Integers m such that the decimal expansion of 1/m contains the
 *          digit 1.
 * https://oeis.org/A353437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   contains_digit_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353437: integers m such that the decimal expansion of 1/m
   contains the digit 1.  
   The function computes the nth term by incremental search; it uses only
   integer arithmetic on seq_t and idx_t. */

#include <stdbool.h>
#include "../utils/contains_digit_one.h"

/* Return the nth term of sequence A353437 (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined for n==0, safe fallback */

    idx_t found = 0;                          /* number of terms found so far */
    seq_t m = (seq_t)1;                       /* current candidate */

    while (true)
    {
        if (contains_digit_one(m))
        {
            found++;
            if (found == n) return m;
        }
        m++;                                   /* next candidate */
    }
}
