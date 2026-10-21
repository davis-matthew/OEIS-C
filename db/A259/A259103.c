/*
 * A259103  Number of steps to get from n to 1 by process of replacing with
 *          the Euler totient function phi if the result is odd, or dividing
 *          by 2 if even.
 * https://oeis.org/A259103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 577, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=0; for n>1,
   let m = (n even) ? n/2 : φ(n); then a(n) = 1 + a(m)   */
#include <stdint.h>

/* Euler's totient function.  The result fits in seq_t for the
   values encountered by this sequence. */
static seq_t phi(seq_t m)
{
    seq_t result = m;
    seq_t p;
    for (p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            while (m % p == 0)
                m /= p;
            result -= result / p;
        }
    }
    if (m > 1)
        result -= result / m;
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* a(1) = 0 */

    seq_t steps = 0;
    seq_t cur   = (seq_t)n;

    while (cur != 1)
    {
        if ((cur & 1) == 0)          /* even: divide by 2 */
            cur >>= 1;
        else                          /* odd: replace by φ(cur) */
            cur = phi(cur);
        ++steps;
    }

    return steps;
}
