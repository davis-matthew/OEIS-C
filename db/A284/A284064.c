/*
 * A284064  Numbers whose smallest decimal digit is 3.
 * https://oeis.org/A284064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 6392, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the nth number whose smallest decimal digit is 3.
   Each number is tested by extracting its decimal digits and checking
   that the minimum digit equals 3.  This method is fast enough for
   n up to about 10 000 (less than a few hundred thousand
   candidate numbers). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0, but safety */
    idx_t   count = 0;             /* how many qualifying numbers seen */
    seq_t   x     = (seq_t)3;      /* start from the first possible candidate */

    for (;; ++x)
    {
        seq_t y   = x;            /* work on a copy for digit extraction */
        seq_t min = 9;            /* digits are 0–9 */

        /* Find the smallest digit of y. */
        while (y > 0)
        {
            seq_t d = y % 10;
            if (d < min) min = d; /* track minimum */
            y /= 10;
        }

        if (min == 3)             /* this number qualifies */
        {
            ++count;
            if (count == n) return x;   /* found the nth term */
        }
    }
}
