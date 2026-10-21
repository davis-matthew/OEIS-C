/*
 * A267830  Number of nonprime numbers in {n, f(n), f(f(n)), ...., 1}, where f
 *          is the Collatz function defined by f(x) = x/2 if x is even; f(x) =
 *          3x + 1 if x is odd.
 * https://oeis.org/A267830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements the definition of OEIS sequence A267830:
   it simulates the Collatz (3x+1) trajectory of n until reaching 1,
   counting the number of values that are not prime. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    idx_t cur = n;
    seq_t count = 0;

    while (1) {
        if (!is_prime(cur))
            count++;

        if (cur == 1)
            break;

        if ((cur & 1) == 0) {          /* even */
            cur >>= 1;
        } else {                        /* odd */
            /* use 128-bit intermediate to avoid overflow */
            cur = (idx_t)((__int128)cur * 3 + 1);
        }
    }
    return count;
}
