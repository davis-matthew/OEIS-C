/*
 * A277249  Largest number that is n times sum of its digits when written in
 *          base 3.
 * https://oeis.org/A277249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = largest number x such that x = n * s_3(x); If no such x
 *            exists (n in A058899) the function returns 0
 * Range:     u8 n <= 36, u16 n <= 4099, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t max = 0;
    seq_t tn = (seq_t)n;

    for (seq_t s = 1; ; ++s) {
        /* Compute x = n * s using the big‑int multiply routine if needed */
        seq_t x = seq_mul(tn, s);

        /* Digit sum of x in base‑3 and the number of base‑3 digits */
        seq_t v   = x;
        seq_t sum = 0;
        size_t  d  = 0;
        while (v > 0) {
            sum += v % 3;
            v   /= 3;
            ++d;
        }

        if (sum == s && x > max)
            max = x;

        /* Once s exceeds the maximum possible digit sum 2*d,
           no further solutions are possible. */
        if (s > (seq_t)(2 * d))
            break;

        /* Safety guard against pathological inputs */
        if (s > 1000000)
            break;
    }

    return max;
}
