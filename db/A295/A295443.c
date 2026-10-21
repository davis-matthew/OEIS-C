/*
 * A295443  a(n) = (18*n)!*(4*n)!/((12*n)!*(9*n)!*n!).
 * https://oeis.org/A295443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (18*n)!*(4*n)!/((12*n)!*(9*n)!*n!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 11,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res = (seq_t)1;
    const int NUM_COUNT = 22;   /* 18 + 4 */
    const int DEN_COUNT = 22;   /* 12 + 9 + 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t numer[NUM_COUNT];
        seq_t denom[DEN_COUNT];

        /* numerator: 18k-17 .. 18k   followed by 4k-3 .. 4k */
        for (int i = 0; i < 18; ++i)
            numer[i] = (seq_t)(18 * k - 17 + i);
        for (int i = 0; i < 4; ++i)
            numer[18 + i] = (seq_t)(4 * k - 3 + i);

        /* denominator: 12k-11 .. 12k   followed by 9k-8 .. 9k
                         and finally k                     */
        for (int i = 0; i < 12; ++i)
            denom[i] = (seq_t)(12 * k - 11 + i);
        for (int i = 0; i < 9; ++i)
            denom[12 + i] = (seq_t)(9 * k - 8 + i);
        denom[DEN_COUNT - 1] = (seq_t)k;

        /* Cancel and accumulate */
        for (int i = 0; i < NUM_COUNT; ++i)
        {
            seq_t num = numer[i];

            /* cancel num with all denominators first */
            for (int j = 0; j < DEN_COUNT; ++j)
            {
                if (denom[j] == 1)
                    continue;
                seq_t g = gcd(num, denom[j]);
                if (g > 1)
                {
                    num /= g;
                    denom[j] /= g;
                    if (num == 1)
                        break;
                }
            }

            /* multiply result by the reduced numerator */
            res *= num;

            /* reduce result with remaining denominators */
            for (int j = 0; j < DEN_COUNT; ++j)
            {
                if (denom[j] == 1)
                    continue;
                seq_t g = gcd(res, denom[j]);
                if (g > 1)
                {
                    res /= g;
                    denom[j] /= g;
                }
            }
        }

        /* any leftover denominators (should be 1) are divided out */
        for (int j = 0; j < DEN_COUNT; ++j)
        {
            if (denom[j] > 1)
            {
                res /= denom[j];
                denom[j] = (seq_t)1;
            }
        }
    }

    return res;
}
