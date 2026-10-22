/*
 * A339690  Positive integers of the form 4^i*9^j*k with gcd(k,6)=1.
 * https://oeis.org/A339690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer that can be written as 4^i * 9^j
 *            * k with integers i, j >= 0 and gcd(k,6)=1. This is equivalent
 *            to numbers whose exponents of 2 and 3 are both even
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)            /* the sequence is defined from index 1 */
        return 0;

    idx_t count = 0;        /* number of qualifying integers found so far */

    for (idx_t m = 1; ; ++m)
    {
        idx_t t = m;
        bool even2 = true;   /* true iff the current exponent of 2 is even */

        /* Count the parity of the exponent of 2 */
        while ((t & 1) == 0) {    /* while t is divisible by 2 */
            t >>= 1;              /* remove one factor of 2 */
            even2 = !even2;       /* flip parity */
        }

        bool even3 = true;   /* true iff the current exponent of 3 is even */

        /* Count the parity of the exponent of 3 */
        while (t % 3 == 0) {
            t /= 3;               /* remove one factor of 3 */
            even3 = !even3;       /* flip parity */
        }

        if (even2 && even3) {
            ++count;
            if (count == n)       /* we have found the nth term */
                return (seq_t)m;
        }
    }
}
