/*
 * A255763  Odd numbers that are not twin primes.
 * https://oeis.org/A255763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th odd integer m such that either m is composite
 *            or neither m-2 nor m+2 is prime. The sequence A255763 lists
 *            these odd numbers in increasing order
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    idx_t count = 0;      /* How many qualifying odds we have seen */
    seq_t  m = 1;         /* Current odd integer being tested   */

    while (1) {
        /* Test whether m itself is prime. */
        bool prime = is_prime((idx_t)m);

        /* Determine whether m participates in a twin prime pair. */
        bool twin = false;
        if (prime) {
            if (m >= 3) {
                /* Check the lower twin (m-2). */
                if (is_prime((idx_t)(m - 2)))
                    twin = true;
            }
            /* Check the upper twin (m+2) only if the lower one failed. */
            if (!twin && is_prime((idx_t)(m + 2)))
                twin = true;
        }

        /* m qualifies if it is not a prime twin. */
        if (!prime || !twin) {
            count++;
            if (count == n)
                return m;
        }

        /* Move to the next odd integer. */
        m += 2;
    }
}
