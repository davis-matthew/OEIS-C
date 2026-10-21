/*
 * A268273  Negabinary odious numbers (see comment).
 * https://oeis.org/A268273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A268273 – Negabinary odious numbers.
   The function returns the n-th positive integer whose
   base –2 representation contains an odd number of 1 bits.
   (Brute‑force search: iterate over candidates, test parity.) */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined case, return 0 */

    idx_t count = 0;
    for (idx_t num = 1; ; ++num)
    {
        /* Count parity of 1's in the negabinary representation of num */
        bool odd = false;
        long long cur = (long long)num;  /* signed for negative division */

        while (cur != 0)
        {
            long long r = cur % -2LL;   /* remainder 0 or –1 */
            cur /= -2LL;
            if (r < 0) {                /* adjust to 0/1 */
                r += 2;
                cur += 1;
            }
            if (r == 1)
                odd = !odd;
        }

        if (odd)
        {
            ++count;
            if (count == n)
                return (seq_t)num;
        }
    }
}
