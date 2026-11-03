/*
 * A343110  Numbers having no divisor of the form 8*k + 7.
 * https://oeis.org/A343110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 147, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9640 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having no divisor of the form 8*k + 7.  The sequence is
   obtained by testing each positive integer in turn; a number is kept
   if no divisor d satisfies d % 8 == 7. */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff x has no divisor congruent to 7 modulo 8. */
static bool has_no_bad_divisor(uint64_t x)
{
    /* Loop over all divisors d <= sqrt(x).  For each divisor, test
       both d and x/d. */
    for (uint64_t d = 1; d <= x / d; ++d)
    {
        if (x % d == 0)
        {
            if (d % 8 == 7)
                return false;
            uint64_t q = x / d;
            if (q != d && q % 8 == 7)
                return false;
        }
    }
    return true;
}

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for n==0, return 0 to keep API consistent */
        return (seq_t)0;

    idx_t count = 0;
    uint64_t cand  = 1;

    for (;;)            /* search until we hit the n-th valid number */
    {
        if (has_no_bad_divisor(cand))
        {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}
