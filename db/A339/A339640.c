/*
 * A339640  a(n) = (A062772(n) + A054270(n)) / 2 - A001248(n).
 * https://oeis.org/A339640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (nextprime(prime(n)^2) + precprime(prime(n)^2))/2 -
 *            prime(n)^2
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/next_prime.h"
#include "../utils/prev_prime.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1; safe guard */

    uint64_t p   = (uint64_t)get_prime(n);      /* nth prime, fits in 32‑bit */
    uint64_t p2  = p * p;                      /* p^2, fits in 64‑bit */

    uint64_t prev = prev_prime(p2);            /* previous prime < p^2 */
    uint64_t next = next_prime(p2);            /* next prime   > p^2 */

    /* use 64‑bit intermediates to avoid overflow when seq_t is smaller */
    int64_t avg = ((int64_t)prev + (int64_t)next) / 2;
    int64_t res = avg - (int64_t)p2;

    return (seq_t)res;
}
