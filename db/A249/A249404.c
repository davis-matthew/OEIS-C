/*
 * A249404  Number of ISO 8601 weeks associated with the Gregorian year n.
 * https://oeis.org/A249404
 *
 * Offset:    2000    (a(2000) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 53 if ISO week 53 occurs in Gregorian year n, otherwise
 *            52
 * Range:     u8 n <= 2399, u16 n <= 2399, u32 n <= 2399, u64 n <= 2399,
 *            u128 n <= 2399, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ISO week 53 occurs when January 1 is Thursday, or when it is Wednesday and the year is leap (Gregorian). */

#include <stdint.h>  /* included for clarity, allowed by compiler */

/* Return the number of ISO‑8601 weeks associated with Gregorian year n.
   The sequence is defined for n ≥ 2000.   */
seq_t a(idx_t n)
{
    if (n < 2000)       /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* Compute day of week for January 1 of year n.
       Sakamoto's algorithm: 0=Sunday, 1=Monday, …, 6=Saturday. */
    idx_t y = n;
    idx_t y2 = y - 1;                 /* month = 1 < 3 so subtract 1 for formula */
    idx_t temp = y2 + y2 / 4 - y2 / 100 + y2 / 400 + 1; /* +day(=1) */
    idx_t w = temp % 7;                /* 0=Sunday … 6=Saturday */

    /* Leap‑year test (Gregorian). */
    idx_t leap = ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0);

    /* ISO week 53 rule. */
    if (w == 4 || (w == 3 && leap))   /* Thursday or Wednesday‑leap year */
        return (seq_t)53;
    else
        return (seq_t)52;
}
