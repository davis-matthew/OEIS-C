/*
 * A250621  a(n) = floor(n*log(prime(n))).
 * https://oeis.org/A250621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * log(prime(n)))
 * Range:     u8 n <= 47, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is 1‑based; the sequence starts at a(1). */

#include <math.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)           /* safety for out‑of‑range indices */
        return (seq_t)0;

    /* get the n‑th prime (1‑based) */
    uint32_t p = get_prime(n);

    /* compute the real value using double precision */
    double v = (double)n * log((double)p);

    /* floor with a tiny epsilon to guard against rounding errors */
    unsigned long long f = (unsigned long long)floor(v + 1e-12);

    /* cast the integer result to seq_t */
    return (seq_t)f;
}
