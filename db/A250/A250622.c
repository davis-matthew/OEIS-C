/*
 * A250622  a(n) = floor(n*log(prime(n)))-prime(n), n >= 1.
 * https://oeis.org/A250622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n*log(prime(n))) - prime(n)
 * Range:     i8 n <= 131, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* get the n-th prime */
    uint64_t p = (uint64_t)get_prime(n);

    /* compute n*log(p) in double precision, add a tiny epsilon to avoid
       rounding‑down errors caused by floating‑point rounding */
    double val = (double)n * log((double)p);
    uint64_t f = (uint64_t)(val + 1e-12);

    /* convert to seq_t and return the required expression */
    return (seq_t)f - (seq_t)p;
}
