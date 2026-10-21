/*
 * A274718  Set x = n. Then a(n) is the number of iterations of successive
 *          applications of the map x = A001414(x) that leave x composite, or
 *          a(n) = -1 if x always remains composite.
 * https://oeis.org/A274718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9424, i16 n <= 9424, i32 n <= 9424, i64 n <= 9424,
 *            i128 n <= 9424, big any n
 * Helpers:   _smallest_prime_factor, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for sequence A274718:
   Start with x = n and repeatedly replace x by the sum of its prime
   factors (with multiplicity).  Count how many times the produced
   value is composite before a prime is reached.  Return -1 if the
   process loops (this happens for n = 1 or 4). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)          /* n = 1 (or 0) stays composite forever */
        return -1;
    uint64_t x = (uint64_t)n;  /* working value */
    seq_t count = 0;           /* number of composite results seen */

    while (true) {
        /* stop if x itself is prime */
        if (is_prime((idx_t)x))
            return count;

        /* compute sum of prime factors of x (with multiplicity) */
        uint64_t sum = 0;
        uint64_t temp = x;
        while (temp > 1) {
            uint64_t p = _smallest_prime_factor(temp);
            sum += p;
            temp /= p;
        }

        /* if the sum is prime, we stop; do not count it */
        if (is_prime((idx_t)sum))
            return count;

        /* detect the only non‑terminating composite fixed point (4) */
        if (sum == x)
            return -1;

        /* otherwise we have produced a composite value */
        count++;
        x = sum;
    }
}
