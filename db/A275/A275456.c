/*
 * A275456  G.f.: 3F2([1/9, 7/9, 8/9], [1/3, 1], 729 x).
 * https://oeis.org/A275456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * a(n) = a(n-1) * * 3*(9*n-8)*(9*n-2)*(9*n-1) / *
 *            (n*n*(3*n-2)) for n >= 1 * Recurrence from OEIS entry A275456
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t res = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* components of the recurrence factor */
        idx_t mul1 = 9 * k - 8;
        idx_t mul2 = 9 * k - 2;
        idx_t mul3 = 9 * k - 1;

        if (sizeof(seq_t) <= sizeof(unsigned long long))   /* built‑in small integer type */
        {
            unsigned __int128 num = 1;
            num *= 3;
            num *= mul1;
            num *= mul2;
            num *= mul3;

            unsigned __int128 den = 1;
            den *= k;
            den *= k;
            den *= (3 * k - 2);

            unsigned __int128 prod = (unsigned __int128)res * num;
            prod /= den;

            res = (seq_t)prod;
        }
        else                                            /* arbitrary‑precision integer type */
        {
            seq_t num = 3;
            num *= mul1;
            num *= mul2;
            num *= mul3;

            seq_t den = k;
            den *= k;
            den *= (3 * k - 2);

            res = res * num / den;
        }
    }

    return res;
}
