/*
 * A353051  Starting with x = n and repeatedly apply the map x -> x - sopfr(x)
 *          until 0, 1 or a prime is reached.
 * https://oeis.org/A353051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = apply x -> x - sopfr(x) until x == 0, 1, or prime (OEIS
 *            A353051)
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   _smallest_prime_factor, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* utility functions (provided by the build system) */
#include "../utils/is_prime_small.h"          /* bool is_prime_small(uint64_t n); */
#include "../utils/_smallest_prime_factor.h" /* uint64_t _smallest_prime_factor(uint64_t n); */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;           /* start with the given n */

    /* handle trivial cases immediately */
    if (cur == 0) return (seq_t)0;
    if (cur == 1) return (seq_t)1;

    while (true) {
        uint64_t cur64 = (uint64_t)cur;

        /* stop if 0, 1, or a prime is reached */
        if (cur64 == 0) return (seq_t)0;
        if (cur64 == 1) return (seq_t)1;
        if (is_prime_small(cur64)) return cur;

        /* compute sopfr(x): sum of prime factors with multiplicity */
        seq_t sopfr = 0;
        uint64_t x = cur64;
        while (x > 1) {
            uint64_t p = _smallest_prime_factor(x);
            sopfr += (seq_t)p;
            x /= p;
        }

        /* step to the next value */
        cur -= sopfr;
    }
}
