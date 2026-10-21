/*
 * A270340  Numbers that equal the sum of the squares of the digits of their
 *          own square in base 13.
 * https://oeis.org/A270340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, base
 * Formula:   a(n) is the nth integer x such that x equals the sum of squares
 *            of the digits of x^2 when x^2 is written in base 13. Computed by
 *            exhaustive search up to the maximum representable value of x,
 *            with the results cached after the first call
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t solutions[1024];
    static size_t sol_len = 0;
    static bool initialized = false;
    if (!initialized)
    {
        initialized = true;
        const idx_t LIMIT = 1000000;   /* safe upper bound for search */
        for (idx_t x = 0; x <= LIMIT; ++x)
        {
            seq_t seqx = (seq_t)x;
            if (seqx != x) break;     /* exceeded seq_t range */
            seq_t sq = seqx * seqx;
            seq_t sum = 0;
            seq_t t = sq;
            while (t > 0)
            {
                seq_t d = t % 13;
                sum += d * d;
                t /= 13;
            }
            if (seqx == sum)
            {
                if (sol_len < 1024)
                    solutions[sol_len++] = seqx;
            }
        }
    }
    if (n == 0) return (seq_t)0;           /* undefined, safe default */
    size_t idx = (size_t)(n - 1);
    if (idx < sol_len)
        return solutions[idx];
    return (seq_t)0;                       /* requested index beyond known terms */
}
