/*
 * A271840  Primes of the form n^3 + 2n^2 + 5n + 11.
 * https://oeis.org/A271840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 2*n^2 + 5*n + 11 (only when the value is prime)
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 21, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* below the offset – undefined */

    idx_t found = 0;                /* number of prime values found so far */
    idx_t m = 0;                    /* current candidate n */

    while (1) {
        seq_t mt = (seq_t)m;
        seq_t val = mt * mt * mt
                  + (seq_t)2 * mt * mt
                  + (seq_t)5 * mt
                  + (seq_t)11;

        if (is_prime_big(val)) {
            ++found;
            if (found == n) return val;
        }
        ++m;
    }
}
