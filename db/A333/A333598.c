/*
 * A333598  Numbers m such that m! has a palindromic number of decimal digits.
 * https://oeis.org/A333598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: generate successive m and record those for which the number
   of decimal digits in m! is a palindrome.  The result a(n) is the nth
   such m (a(1)=0).  The implementation keeps a static array of terms
   and iterates m until the requested index is reached. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Allocate a generous static buffer for the sequence.  This size is
   sufficient for n up to about 10 000.  It works for both 64‑bit
   integer builds and arbitrary‑precision builds. */
#define MAX_TERMS 200000

/* Static storage for the terms. */
static seq_t seq[MAX_TERMS];
static size_t seq_len = 0;        /* number of terms stored */

/* The current m being examined; logsum holds Σ log10(i) for i = 1 .. m-1. */
static idx_t cur_m = 0;
static long double logsum = 0.0L;

/* Return true iff x is a decimal palindrome. */
static bool is_palindrome_idx(idx_t x)
{
    if (x < 10) return true;
    idx_t rev = 0, orig = x;
    while (x > 0) {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    }
    return rev == orig;
}

/* a(n) returns the nth term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms. */
    while (seq_len < (size_t)n) {
        /* Number of decimal digits in cur_m!. */
        idx_t digits = (idx_t)logsum + 1;   /* floor(logsum)+1 */
        if (is_palindrome_idx(digits)) {
            if (seq_len == MAX_TERMS) abort();   /* safety check */
            seq[seq_len++] = (seq_t)cur_m;
        }
        /* Advance to next m. */
        ++cur_m;
        logsum += log10l((long double)cur_m);
    }
    return seq[n - 1];
}
