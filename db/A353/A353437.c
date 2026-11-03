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
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353437: integers m such that the decimal expansion of 1/m
   contains the digit 1.  
   The function computes the nth term by incremental search; it uses only
   integer arithmetic on seq_t and idx_t. */

#include <stdbool.h>

/* Return true if the decimal expansion of 1/m contains the digit '1'.
   The routine performs the usual long‑division algorithm and stops
   immediately when the digit 1 is generated, or when the remainder
   becomes zero.  It limits the number of steps to m, which is the
   maximum length of the repeating cycle.             */
static bool contains_digit_one(seq_t m)
{
    if (m == (seq_t)1) return true;           /* 1/1 = 1.0 contains 1 */

    seq_t r = (seq_t)1 % m;                   /* remainder after the integer part */
    idx_t limit = (idx_t)m;                   /* number of steps needed to complete a cycle */

    for (idx_t step = 0; step < limit && r != 0; ++step)
    {
        seq_t r10 = r * (seq_t)10;            /* multiply remainder by 10 */
        seq_t digit = r10 / m;                /* next decimal digit */
        if (digit == (seq_t)1) return true;   /* digit 1 found */
        r = r10 % m;                          /* update remainder */
    }
    return false;                             /* no digit 1 in the expansion */
}

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
