/*
 * A248912  a(n) = floor(prime(n) - (n+1)*(log(n) + log(log(n))) + n) for n >
 *          1.
 * https://oeis.org/A248912
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor(prime(n) - (n+1)*(log(n)+log(log(n))) + n)
 * Range:     i8 n <= 1399, i16 n <= 9918, i32 n <= 9918, i64 n <= 9918,
 *            i128 n <= 9918, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For indices below the offset the value is irrelevant */
    if (n < 2) return 0;

    /* Obtain the n-th prime (1‑based).  get_prime() returns uint32_t. */
    uint32_t prime_n = get_prime(n);

    /* Compute the expression using long double precision. */
    long double nd   = (long double)n;
    long double ln   = log(nd);
    long double lln  = log(ln);
    long double expr = (long double)prime_n
                      + nd
                      - (nd + 1.0L) * (ln + lln);

    /* The result fits into a signed integer of seq_t size. */
    return (seq_t)(int64_t)floorl(expr);
}
