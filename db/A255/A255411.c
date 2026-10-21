/*
 * A255411  Shift factorial base representation of n one digit left (with 0
 *          added to right), increment all nonzero digits by one, then convert
 *          back to decimal; Numbers with no digit 1 in their factorial base
 *          representation.
 * https://oeis.org/A255411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 5039, u32 n <= 9961, u64 n <= 9961,
 *            u128 n <= 9961, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A255411: shift left factorial‑base representation, increment
   all non‑zero digits by one, and convert back to decimal.
   The sequence consists of all nonnegative integers whose
   factorial‑base representation contains no digit 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Digits of n in factorial base; dig[0] = coefficient of 1!, dig[1] of 2!, … */
    unsigned char dig[32];
    unsigned int k = 0;          /* number of digits found */
    idx_t rem = n;

    /* Decompose n into factorial base digits */
    while (rem) {
        /* The divisor for the next digit is (k+2) because the first divisor is 2 (for 1!).
           For k=0 (first digit) divisor = 2; for k=1 divisor = 3; etc. */
        unsigned int div = k + 2;
        dig[k] = (unsigned char)(rem % div);
        rem /= div;
        ++k;
    }

    /* Reconstruct the factorial representation after shifting left
       and incrementing all non‑zero digits. */
    seq_t result = 0;
    seq_t fact   = 1;           /* current factorial i! (starting with 1! = 1) */

    /* We need to iterate from i = 1 up to k+1:
       i = 1 corresponds to 1! and its digit is always 0 (the appended zero). */
    for (unsigned int i = 1; i <= k + 1; ++i) {
        seq_t digit = 0;
        if (i > 1) {                 /* no need to access dig[-1] */
            if (dig[i - 2] != 0) {
                digit = (seq_t)dig[i - 2] + 1;   /* increment non‑zero digits */
            }
        }
        result += digit * fact;
        fact *= (seq_t)(i + 1);      /* advance to (i+1)! for next iteration */
    }

    return result;
}
