/*
 * A322885  Number of 3-generated Abelian groups of order n.
 * https://oeis.org/A322885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} floor((e^2+6e+12)/12) (number of 3‑generated
 *            Abelian groups of order n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   partitions_le3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/partitions_le3.h"

/* --------------------------------------------------------------- */
/*  Main sequence function                                         */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    uint64_t v = (uint64_t)n;
    if (v <= 1)          /* a(1) = 1, a(0) unused */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t t = v;

    for (uint64_t i = 2; i <= t / i; ++i)
    {
        if (t % i == 0)
        {
            uint64_t exp = 0;
            do
            {
                t /= i;
                ++exp;
            } while (t % i == 0);
            result *= partitions_le3(exp);
        }
    }

    if (t > 1)           /* remaining prime factor with exponent 1 */
        result *= partitions_le3(1);

    return result;
}
