/*
 * A251702  a(1)=5, a(n) = a(n-1)*(a(n-1)-1)*(a(n-1)-2)/6.
 * https://oeis.org/A251702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 5, a(n) = binomial(a(n-1),3) =
 *            a(n-1)*(a(n-1)-1)*(a(n-1)-2)/6 (n>=2)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1; return 0 to satisfy compiler */
        return 0;

    seq_t res = 5;      /* a(1) */

    /* For n > 1 compute recursively: res = res*(res-1)*(res-2)/6 */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t a_factor = res;
        seq_t b_factor = res - 1;
        seq_t c_factor = res - 2;

        /* Divide by 2, then by 3, to prevent overflow. */
        if ((a_factor & 1) == 0)            /* a_factor even */
            a_factor /= 2;
        else if ((b_factor & 1) == 0)
            b_factor /= 2;
        else
            c_factor /= 2;

        if (a_factor % 3 == 0)
            a_factor /= 3;
        else if (b_factor % 3 == 0)
            b_factor /= 3;
        else
            c_factor /= 3;

        /* Multiply the (now reduced) factors */
        res = a_factor * b_factor * c_factor;
    }

    return res;
}
