/*
 * A246660  Run Length Transform of factorials.
 * https://oeis.org/A246660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 510, u32 n <= 8190, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Run‑length transform of factorials:
 * a(n) = product of factorials of the lengths of all runs of 1’s in
 * the binary expansion of n.
 * Recurrence: a(2n) = a(n);  a(2n+1) = a(n)*A007814(2n+2). */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;

    /* Process the binary representation of n, alternating
     * between runs of 0’s and runs of 1’s. */
    while (n > 0)
    {
        /* Skip trailing zeros – they contribute nothing. */
        while ((n & 1ULL) == 0ULL) {
            n >>= 1;
            if (n == 0)
                return result;
        }

        /* Count the current run of 1’s. */
        idx_t run = 0;
        while ((n & 1ULL) == 1ULL) {
            ++run;
            n >>= 1;
        }

        /* Multiply by factorial(run). Compute factorial locally
         * to avoid external dependencies. */
        seq_t f = (seq_t)1;
        for (idx_t i = 2; i <= run; ++i)
            f *= (seq_t)i;

        result *= f;
    }

    return result;
}
