/*
 * A286575  Run-length transform of A001316.
 * https://oeis.org/A286575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product over the lengths L of the 1-runs in binary(n) *
 *            of A001316(L) = 2^popcount(L). Equivalently * a(n) =
 *            2^sum_{runs} popcount(run length). * * This implements the
 *            run‑length transform of A001316 as defined in A286575
 * Range:     u8 n <= 9714, u16 n <= 9714, u32 n <= 9714, u64 n <= 9714,
 *            u128 n <= 9714, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

/* Count the number of 1 bits in an unsigned integer. */
static idx_t popcount_u64(idx_t x)
{
    idx_t c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    idx_t run = 0;           /* current length of a run of 1s */
    idx_t exponent = 0;      /* sum of popcounts of all completed runs */

    while (n)
    {
        if (n & 1)
        {
            ++run;                 /* extend current run */
        }
        else if (run)
        {
            exponent += popcount_u64(run);
            run = 0;              /* run finished */
        }
        n >>= 1;
    }

    /* If the number ends with a run of 1s, add its contribution */
    if (run)
        exponent += popcount_u64(run);

    return pow2(exponent); /* 2^exponent */
}
